#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DFarmCrop.h"
#include "DCarrot.generated.h"

UCLASS()
class PROJECTD_API ADCarrot : public ADFarmCrop
{
	GENERATED_BODY()
	
public:	
	ADCarrot();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
