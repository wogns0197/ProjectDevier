#include "DCharacterAnimation.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../DCharacter.h"

void UDCharacterAnimation::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UDCharacterAnimation::NativeUpdateAnimation( float DeltaSeconds )
{
	Super::NativeUpdateAnimation( DeltaSeconds );
	UpdateAnimProps();
}

void UDCharacterAnimation::UpdateAnimProps()
{
	IsWalking();
	IsRunning();
	IsJumping();
	IsFalling();
	IsPicking();
}

void UDCharacterAnimation::IsWalking()
{
	if ( ADCharacter* pCharacter = GetCharacter() )
	{
		bWalking = pCharacter->GetVelocity().X != 0.f || pCharacter->GetVelocity().Y != 0.f;
	}
}

void UDCharacterAnimation::IsRunning()
{
	if ( ADCharacter* pCharacter = GetCharacter() )
	{
		bRunning = ( pCharacter->GetVelocity().X != 0.f || pCharacter->GetVelocity().Y != 0.f ) && pCharacter->GetIsPressingShift();
	}
}

void UDCharacterAnimation::IsJumping()
{
	if ( ADCharacter* pCharacter = GetCharacter() )
	{
		bJumping = pCharacter->GetIsPressedJump();
	}
}

void UDCharacterAnimation::IsFalling()
{
	if ( ADCharacter* pCharacter = GetCharacter() )
	{
		bFalling = pCharacter->GetCharacterMovement()->IsFalling();
	}
}

void UDCharacterAnimation::IsPicking()
{
	if ( ADCharacter* pCharacter = GetCharacter() )
	{
		bPicking = pCharacter->GetIsPickStart();
	}
}

ADCharacter* UDCharacterAnimation::GetCharacter()
{
	return Cast<ADCharacter>( GetOwningActor() );
}
