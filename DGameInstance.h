#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DGameInstance.generated.h"

enum class EInteractiveType;
// Interact Flow
// AnimNotify -> Character -> GameInstance Manage Character, InteractiveObject -> GI notify to Character, Destroy Object, Save Inventory
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FDelegateDelayedInteractDone, EInteractiveType, Type );

UENUM()
enum class EInventoryType
{
	Equip = 0,
	Consume,
	Crops,
	Etc,
	Cash,
	COUNT,
};

struct FInventoryProcessParam
{
	EInventoryType Type;
	bool bAdd;
	int ItemID;
	int Num;
	int Money;
	FInventoryProcessParam( EInventoryType InType, bool InbAdd, int InItemID, int InNum, int InMoney )
	{
		Type = InType;
		bAdd = InbAdd;
		ItemID = InItemID;
		Num = InNum;
		Money = InMoney;
	};
};

USTRUCT()
struct FInventoryItem
{
	GENERATED_USTRUCT_BODY()

	int ItemID;
	int Num;
	FInventoryItem() : ItemID( 0 ), Num( 0 ) {};
	FInventoryItem( int InItemID, int InNum )
	{
		ItemID = InItemID;
		Num = InNum;
	}
};

UCLASS()
class PROJECTD_API UDGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int InventoryCountByType; // 타입별 최대 인벤토리 갯수

private:
	UPROPERTY()
	class ADCharacter* m_Character;

	TArray<TArray<FInventoryItem>> m_Inventory;

	TMap<int, UTexture2D*> ThumbnailsArr;

public:
	FDelegateDelayedInteractDone DelayedInteractDoneDelegate;

public:
	virtual void Init() override;
	void InitCharacterData( class ADCharacter* pOwnCharacter );
	void ProcessInteractive( EInteractiveType InType, TWeakObjectPtr<class ADInteractiveObject> InObject );

	// ONLY CALL FROM INVENTORY!! 객체 관리를 아주 제한적으로 하고싶기 때문에 인벤토리에서만 어쩔 수 없이 가져온다.
	const TArray<FInventoryItem>& GetInventoryBag( EInventoryType Type ) { return m_Inventory[(int)Type]; }

	UTexture2D* GetThumbnail( int InItemID );

private:
	void LoadAllTexturesInDirectory( const FString& Directory );
	void LoadSpriteAsync( const FString& AssetPath );
	bool ProcessInventory( const FInventoryProcessParam& Param );
	void UpdateInventory();
};
