#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "DAnimNotifyState.generated.h"

UCLASS()
class UDAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference ) override;
	virtual void NotifyTick( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference ) override;
	virtual void NotifyEnd( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation ) override;

};
