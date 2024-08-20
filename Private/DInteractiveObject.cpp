#include "DInteractiveObject.h"
#include "Components/SceneCaptureComponent.h"

ADInteractiveObject::ADInteractiveObject()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneCaptureComponent>( TEXT( "SceneComp" ) );
	if ( SceneComp )
	{
		StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "StaticMesh" ) );
		if ( StaticMeshComp ) {
			StaticMeshComp->SetupAttachment( SceneComp );
		}
	}
}

void ADInteractiveObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADInteractiveObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADInteractiveObject::SetPhysicsSimulate( bool v )
{
	if ( StaticMeshComp ) {
		StaticMeshComp->SetSimulatePhysics( v );
	}
}
