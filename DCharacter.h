#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DCharacter.generated.h"

UCLASS()
class PROJECTD_API ADCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ADCharacter();

public:
	UPROPERTY(VisibleAnywhere)
    class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere)
    class USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* OverlapCapsuleComponent;
	
	UPROPERTY(EditAnywhere, Category="MovementSetting")
	float RotSpeed = 5.0f;
	UPROPERTY(EditAnywhere, Category="MovementSetting")
	float RunSpeedOffset;
	UPROPERTY(EditAnywhere, Category="MovementSetting")
	float DoubleMovementTimeOffset; // 이동키 연타 판별 시간
	UPROPERTY(EditAnywhere, Category="MovementSetting")
	float PickObjectTime; // 줍기 애님 직후 이 시간이 지나야 오브젝트가 캐릭터 소켓에 붙는다.
	//UPROPERTY(EditAnywhere, Category="MovementSetting")
	//float WalkToTargetTime; // 특정 타겟으로 걸어가기까지의 시간 : 시간으로 하면 안된다. 속도와 거리로 해야함

	UPROPERTY(EditAnywhere, Category="SpringArmSetting")
	float ZoomDelta;
	UPROPERTY(EditAnywhere, Category="SpringArmSetting")
	float ZoomSpeed;
	UPROPERTY(EditAnywhere, Category="SpringArmSetting")
	float MaxArmLength;
	UPROPERTY(EditAnywhere, Category="SpringArmSetting")
	float MinArmLength;

private:
	float dBaseWalkSpeed;
	float dCurArmLength;
	bool bPressingShift;
	bool bAbleDoublePressedRun; // 움직이기 키 두번으로 뛸 수 있는 연타간 시간 간격 검사
	bool bMoveable; // 캐릭터 무빙 잠금 (ex 줍기 시)

	// 삭제예정 : 마우스 클릭 이동으로 바꾼 다음 구현해야함
	//float MoveToTargetElapsedTime; // 아으 기능별로 묶을걸..담부턴 기능별로 한정자 묶자
	//FVector MoveToTargetStartVec;
	//FVector MoveToTargetFinishVec;
	//bool* AnimBooladdr; // 함수에 Animbool 주솟값으로 던져주고 나중에 플래그 올려주는데 더 좋은 방법이 없을까.. enum으로 애니메이션을 관리해야할까..
	//TWeakObjectPtr<class ADInteractiveObject> MoveTargetObject;

	bool Anim_bPressingJump;
	bool Anim_bPickable;
	bool Anim_bPunching;
	bool Anim_Walking; // 걷기 애니메이션은 기본적으로 속도로 판별하나, 코드에서 직접 캐릭터를 움직일 때가 있어서 강제로 애님을 부여할때만 사용

	// 오브젝트 줍기 애님 중 땅에 손이 닿는 타이밍(사실 에님 노티파이로 하면 됨 ㅋㅋ;;)
	FTimerHandle PickTimeHandler;

public:
	const bool GetIsPressingShift() { return bPressingShift; }
	const bool GetIsPressedJump() { return Anim_bPressingJump; }
	const bool GetIsPickStart() { return Anim_bPickable; } // 주의* 애니메이션 Update에서 계속 바라보고 있음
	const bool GetIsPunchStart() { return Anim_bPunching; } // 위와 동일
	const bool GetIsWaking() { return Anim_Walking; } // 강제로 코드로 움직일 때만 사용

public: // DAnimNotifyState에서 관리한다.
	void SetIsPickStart( const bool v ) { Anim_bPickable = v; }
	void SetIsPunching( const bool v ) { Anim_bPunching = v; }

	// 흠.. 오버랩되는 물체들이 한번에 여러개가 있지 않을까.. 맵으로 현재 오버랩타입을 관리하고픈데..
	// 역시 여러개가 있게 되어 작업함
	TMap<EInteractiveType, TArray<TWeakObjectPtr<class ADInteractiveObject>>> m_mInteractiveArr;
	TWeakObjectPtr<class ADInteractiveObject> WeakCurOverlapObject;
	class UAnimInstance* AnimInstance;

	FTimerHandle DoubleMovePressedTimerHandle;

// UI
private:
	UPROPERTY()
	class UDUIInventory* m_pUIInventory;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void UpdateInventory();

public:
	UFUNCTION()
	void LookUp(float v);
	UFUNCTION()
	void Turn(float v);
	UFUNCTION()
	void MoveForward(float v);
	UFUNCTION()
	void MoveRight(float v);
	UFUNCTION()
	void OnMouseWheel(float v);
	UFUNCTION()
	void OnShiftPressed();
	UFUNCTION()
	void OnShiftReleased();
	UFUNCTION()
	void OnJumpPressed();
	UFUNCTION()
	void OnJumpRepeated();
	UFUNCTION()
	void OnJumpReleased();
	UFUNCTION()
	void OnInteractivePressed();
	UFUNCTION()
	void OnInventoryKeyPressed();
	// 움직이기 키 연타 검사
	/*UFUNCTION()
	void OnPressedMoveD();
	UFUNCTION()
	void OnReleasedMoveD();*/

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void OnNotifyAnimDone( EInteractiveType InType );
	UFUNCTION()
	void OnInteractiveProcessDone(EInteractiveType InType);

private:
	class ADHUDBase* ADCharacter::GetHud();
	void RotateToTarget( FVector vec );
	void RotateToDirection( FVector RotVec );
	void MoveToTarget( TWeakObjectPtr<class ADInteractiveObject> TargetObj, bool& InAnimBoolAddr );
	FVector GetClosetDistanceToTarget( AActor* Obj ); // 다른 액터에도 쓰일 것 같아서 함수 안에서 캐스팅해서 쓰도록함
	void CheckUnMovableState( EInteractiveType InteractiveType /*분명 무언가 또 추가될 것임*/);

	void SetFocusToMouseCursorAndShow( bool bFocus ); // UI 오픈 시 카메라가 마우스 움직임을 받을지 안받을지 + 커서 Show
	class ADInteractiveObject* GetRemainInteractObject();

};
