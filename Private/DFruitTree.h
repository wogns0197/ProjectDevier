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
	float ThrowFruitDelayTime; // Interact���ĺ��� ���϶���߸������ �ð�
	UPROPERTY(EditAnywhere)
	int LengthGenerateFruit; // �����κ��� ���ϻ�����ġ������ ���� ( 4���� ���� )

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* OverlapCapsuleComponent; // ������
	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* BlockCapsuleComponent; // ĳ���� Block
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent * TreeSkeletalMesh;
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

private:
	FTimerHandle ThrowTimeHanle;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void OnStartInteractive() override;
};
