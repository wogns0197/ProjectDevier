#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DCharacterAnimation.generated.h"

UCLASS()
class PROJECTD_API UDCharacterAnimation : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation( float DeltaSeconds ) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Anim Props")
	bool bWalking;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Anim Props")
	bool bRunning;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Anim Props")
	bool bJumping;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Anim Props")
	bool bFalling;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Anim Props")
	bool bPicking;

public:
	UFUNCTION(BlueprintCallable , Category="Anim Func")
	void UpdateAnimProps();

	void IsWalking();
	void IsRunning();
	void IsJumping();
	void IsFalling();
	void IsPicking();


private:
	class ADCharacter* GetCharacter();
};
