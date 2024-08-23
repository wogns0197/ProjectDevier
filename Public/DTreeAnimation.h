#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DTreeAnimation.generated.h"

UCLASS()
class PROJECTD_API UDTreeAnimation : public UAnimInstance // 이거 DAnimInstance로 상위 부모 상속구조 만들자
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
