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
	bool Anim_bPressingJump;
	bool Anim_bPickable;
	bool bAbleDoublePressedRun; // 움직이기 키 두번으로 뛸 수 있는 연타간 시간 간격 검사
	bool bMoveable; // 캐릭터 무빙 잠금 (ex 줍기 시)

	// 오브젝트 줍기 애님 중 땅에 손이 닿는 타이밍(사실 에님 노티파이로 하면 됨 ㅋㅋ;;)
	FTimerHandle PickTimeHandler;

public:
	const bool GetIsPressingShift() { return bPressingShift; }
	const bool GetIsPressedJump() { return Anim_bPressingJump; }
	const bool GetIsPickStart() { return Anim_bPickable; } // 주의* 애니메이션 Update에서 계속 바라보고 있음
	void SetIsPickStart( const bool v ) { Anim_bPickable = v; } // DAnimNotifyState에서 관리한다.

	// 흠.. 오버랩되는 물체들이 한번에 여러개가 있지 않을까.. 맵으로 현재 오버랩타입을 관리하고픈데..
	TWeakObjectPtr<class ADInteractiveObject> WeakCurOverlapObject;
	class UAnimInstance* AnimInstance;

	FTimerHandle DoubleMovePressedTimerHandle;

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
	void CheckUnMovableState( EInteractiveType InteractiveType /*분명 무언가 또 추가될 것임*/);

};
