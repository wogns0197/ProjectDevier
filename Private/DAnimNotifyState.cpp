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
	if ( ADCharacter* pCharacter = Cast<ADCharacter>( MeshComp->GetOwner() ) )
	{
		EInteractiveType BroadCastType = EInteractiveType::NONE;

		if ( Animation->GetName() == TEXT( "Anim_Pickup" ) )
			BroadCastType = EInteractiveType::Picking;
		else if ( Animation->GetName() == TEXT( "Anim_Punching" ) ) // ÆÝÄ¡°¡ °ð Trembling
			BroadCastType = EInteractiveType::Trembling;

		pCharacter->OnNotifyAnimDone( BroadCastType );
	}
}