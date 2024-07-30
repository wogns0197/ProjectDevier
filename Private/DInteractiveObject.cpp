#include "DInteractiveObject.h"

ADInteractiveObject::ADInteractiveObject()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADInteractiveObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADInteractiveObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

