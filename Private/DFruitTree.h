#pragma once

#include "CoreMinimal.h"
#include "DInteractiveObject.h"
#include "DFruitTree.generated.h"

UCLASS()
class ADFruitTree : public ADInteractiveObject
{
	GENERATED_BODY()
	
public:	
	ADFruitTree();

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ADFarmCrop> FruitCropClass;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UStaticMesh> FruitMeshClass;

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* OverlapCapsuleComponent; // 오버랩
	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* BlockCapsuleComponent; // 캐릭터 Block
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* TreeStaticMesh;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* FruitStaticMesh_0;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* FruitStaticMesh_1;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* FruitStaticMesh_2;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* FruitStaticMesh_3;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* FruitStaticMesh_4;


protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
