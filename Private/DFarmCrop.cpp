#include "DFarmCrop.h"
#include "CommonEnum.h"

ADFarmCrop::ADFarmCrop()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "StaticMesh" ) );
	if ( StaticMeshComp ) {
		StaticMeshComp->SetupAttachment( RootComponent );
	}
}

void ADFarmCrop::BeginPlay()
{
	Super::BeginPlay();
	
	OverlapType = EOverlapObjectType::Interactive;
}

void ADFarmCrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADFarmCrop::SetPhysicsSimulate( bool v )
{
	if ( StaticMeshComp ) {
		StaticMeshComp->SetSimulatePhysics( v );
	}
}
