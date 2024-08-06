#pragma once

#include "CoreMinimal.h"
#include "DInteractiveObject.h"
#include "../DGameInstance.h"
#include "DFarmCrop.generated.h"



UCLASS()
class PROJECTD_API ADFarmCrop : public ADInteractiveObject
{
	GENERATED_BODY()
	
public:	
	ADFarmCrop();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


};
