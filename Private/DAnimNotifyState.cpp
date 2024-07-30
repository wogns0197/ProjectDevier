#include "DAnimNotifyState.h"
#include "CommonEnum.h"
#include "../DCharacter.h"

void UDAnimNotifyState::NotifyBegin( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference )
{
}

void UDAnimNotifyState::NotifyTick( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference )
{
}

void UDAnimNotifyState::NotifyEnd( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	if ( Animation->GetName() == TEXT( "Anim_Pickup" ) )
	{
		if ( ADCharacter* pCharacter = Cast<ADCharacter>( MeshComp->GetOwner() ) )
		{
			pCharacter->OnNotifyAnimDone( EInteractiveType::Picking );
		}
	}
}
