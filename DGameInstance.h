#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DGameInstance.generated.h"

enum class EInteractiveType;
enum class EInventoryType;
// Interact Flow
// AnimNotify -> Character -> GameInstance Manage Character, InteractiveObject -> GI notify to Character, Destroy Object, Save Inventory
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FDelegateDelayedInteractDone, EInteractiveType, Type );

UCLASS()
class PROJECTD_API UDGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class ADCharacter* m_Character;

// inventory mgr
public:
	UPROPERTY(EditAnywhere)
	int InventoryCountByType; // 타입별 최대 가방 칸 개수
	UPROPERTY(EditAnywhere)
	TMap<EInventoryType, int> MaxCountByBagItem; // 타입별 아이템 한 칸당 최대 Num

public:
	FDelegateDelayedInteractDone DelayedInteractDoneDelegate;

public:
	virtual void Init() override;
	void InitCharacterData( class ADCharacter* pOwnCharacter );
	void ProcessInteractive( EInteractiveType InType, TWeakObjectPtr<class ADInteractiveObject> InObject );
	bool IsAbleToTremble( TWeakObjectPtr<class ADInteractiveObject> InWeakPtr );
};
