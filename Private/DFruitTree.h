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
	float ThrowFruitDelayTime; // Interact직후부터 과일떨어뜨리기까지 시간
	UPROPERTY(EditAnywhere)
	int LengthGenerateFruit; // 나무로부터 과일생성위치까지의 길이 ( 4방향 랜덤 )

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* OverlapCapsuleComponent; // 오버랩
	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* BlockCapsuleComponent; // 캐릭터 Block
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
