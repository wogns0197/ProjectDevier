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
	float DoubleMovementTimeOffset; // �̵�Ű ��Ÿ �Ǻ� �ð�
	UPROPERTY(EditAnywhere, Category="MovementSetting")
	float PickObjectTime; // �ݱ� �ִ� ���� �� �ð��� ������ ������Ʈ�� ĳ���� ���Ͽ� �ٴ´�.

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
	bool bAbleDoublePressedRun; // �����̱� Ű �ι����� �� �� �ִ� ��Ÿ�� �ð� ���� �˻�
	bool bMoveable; // ĳ���� ���� ��� (ex �ݱ� ��)

	// ������Ʈ �ݱ� �ִ� �� ���� ���� ��� Ÿ�̹�(��� ���� ��Ƽ���̷� �ϸ� �� ����;;)
	FTimerHandle PickTimeHandler;

public:
	const bool GetIsPressingShift() { return bPressingShift; }
	const bool GetIsPressedJump() { return Anim_bPressingJump; }
	const bool GetIsPickStart() { return Anim_bPickable; } // ����* �ִϸ��̼� Update���� ��� �ٶ󺸰� ����
	void SetIsPickStart( const bool v ) { Anim_bPickable = v; } // DAnimNotifyState���� �����Ѵ�.

	// ��.. �������Ǵ� ��ü���� �ѹ��� �������� ���� ������.. ������ ���� ������Ÿ���� �����ϰ��µ�..
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
	// �����̱� Ű ��Ÿ �˻�
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
	void CheckUnMovableState( EInteractiveType InteractiveType /*�и� ���� �� �߰��� ����*/);

};
