#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DInteractiveObject.generated.h"

enum class EOverlapObjectType;
enum class EInteractiveType;

UCLASS()
class PROJECTD_API ADInteractiveObject : public AActor
{
	GENERATED_BODY()
	
protected:
	EOverlapObjectType OverlapType;
	EInteractiveType InteractiveType;

public:	
	ADInteractiveObject();
	const EOverlapObjectType GetOverlapType() { return OverlapType; }
	const EInteractiveType GetInteractiveType() { return InteractiveType; }

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
