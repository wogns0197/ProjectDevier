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

public:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMeshComp;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void OnStartInteractive() {};
	virtual void SetPhysicsSimulate( bool v );
};
