#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DSoilField.generated.h"

UCLASS()
class PROJECTD_API ADSoilField : public AActor
{
	GENERATED_BODY()
public:	
	ADSoilField();
protected:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ADFarmCrop> BP_FarmCrop;
	/*UPROPERTY(EditAnywhere)
	int SpawnDivNum;*/
	UPROPERTY(EditAnywhere)
	int intervalX;
	UPROPERTY(EditAnywhere)
	int intervalY;

private:
	UPROPERTY()
	TArray<class ADFarmCrop*> FarmCropArr;


protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	void InitCrops();

};
