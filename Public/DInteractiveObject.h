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

public:
	UPROPERTY(EditAnywhere, Category="ItemInfo")
	int ItemID;
	UPROPERTY(EditAnywhere, Category="InteractInfo")
	int InteractableOffsetLength; // 오버랩되어서 인터랙트키 누른다음 실제 인터렉트 하기 전까지 걸어가야하는 길이

protected:
	EOverlapObjectType OverlapType;
	EInteractiveType InteractiveType;

public:	
	ADInteractiveObject();
	const EOverlapObjectType GetOverlapType() { return OverlapType; }
	const EInteractiveType GetInteractiveType() { return InteractiveType; }
	const int GetItemID() { return ItemID; }
	const int GetInteractableDistance() { return InteractableOffsetLength; }

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void OnStartInteractive() {}; 
	virtual void SetPhysicsSimulate( bool v ) {}; // 상속받는 자식에서 스태틱메시를 포함해야한다
};
