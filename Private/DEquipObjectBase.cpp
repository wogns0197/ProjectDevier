#include "DEquipObjectBase.h"
#include "../DCharacter.h"

ADEquipObjectBase::ADEquipObjectBase()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "StaticMesh" ) );
	if ( StaticMeshComp ) {
		StaticMeshComp->SetupAttachment( RootComponent );
	}
}

void ADEquipObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADEquipObjectBase::AttachToCharacter( ADCharacter* pCharacter )
{
	if ( !pCharacter->IsValidLowLevel() ) { return; }

	FAttachmentTransformRules AttathRules( EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false );
	AttachToComponent( pCharacter->GetMesh(), AttathRules, "Fist_R_Socket" );
}
