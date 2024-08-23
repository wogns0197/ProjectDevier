#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DTreeAnimation.generated.h"

UCLASS()
class PROJECTD_API UDTreeAnimation : public UAnimInstance // �̰� DAnimInstance�� ���� �θ� ��ӱ��� ������
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Anim Props")
	bool bTremble;

protected:
	virtual void NativeInitializeAnimation() override;
	/*virtual void NativeUpdateAnimation( float DeltaSeconds ) override;*/

public:
	UFUNCTION(BlueprintCallable , Category="Anim Func")
	void StartTrembleAnim();
	
};
