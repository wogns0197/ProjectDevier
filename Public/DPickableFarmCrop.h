#pragma once

#include "CoreMinimal.h"
#include "DFarmCrop.h"
#include "DPickableFarmCrop.generated.h"

UCLASS()
class PROJECTD_API ADPickableFarmCrop : public ADFarmCrop
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
};
