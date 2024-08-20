#include "DFarmCrop.h"
#include "CommonEnum.h"

ADFarmCrop::ADFarmCrop()
{
	PrimaryActorTick.bCanEverTick = true;
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