#include "DCarrot.h"
#include "CommonEnum.h"

ADCarrot::ADCarrot()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADCarrot::BeginPlay()
{
	Super::BeginPlay();

	InteractiveType = EInteractiveType::Picking;
	
}

void ADCarrot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

