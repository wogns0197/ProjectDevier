#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DEquipObjectBase.generated.h"

UCLASS()
class ADEquipObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ADEquipObjectBase();

public:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMeshComp;

protected:
	virtual void BeginPlay() override;

public:	
	void AttachToCharacter( class ADCharacter* pCharacter );

};
