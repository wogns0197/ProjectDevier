#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DInventoryManagerSubSystem.generated.h"

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

USTRUCT() // �� ����ü�� DateSheet���� ����
struct FItemBaseInfo
{
	GENERATED_USTRUCT_BODY()

	FText ItemName;
	FText ToolTip;
	int Price;
	FDateTime ExpireDate;

	FItemBaseInfo() : ItemName( FText::GetEmpty() ), ToolTip( FText::GetEmpty() ), Price( 99 ), ExpireDate( FDateTime( 2079, 1, 1, 12, 59, 59 ) ) {};
};

static UDInventoryManagerSubSystem* _instance;

UCLASS(Blueprintable)
class PROJECTD_API UDInventoryManagerSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void Init( const int nCountByType, const TMap<EInventoryType, int>& mMaxCountByType );
	virtual void Initialize( FSubsystemCollectionBase& Collection ) override;
	virtual void Deinitialize() override;

	static UDInventoryManagerSubSystem* GetInstance();


public:
	int InventoryCountByType;
	TMap<EInventoryType, int> MaxCountByBagItem;
	UPROPERTY(EditAnywhere)
	TMap<int, FItemBaseInfo> m_mItemInfo;

private:
	TArray<TArray<FInventoryItem>> m_Inventory;
	TMap<int, UTexture2D*> ThumbnailsArr;

public:
	EInventoryType LastInventoryUIBagType; // �κ��丮�� �ٽ� �� �� ������ �ݾҴ� ������ �ٽ� ������
	
public:
	bool IsInventoryPuttable( TWeakObjectPtr<class ADInteractiveObject> InWeakPtr ); // �κ� �ڸ� �ִ���, �κ��� ���� �� �ִ� ��ü���� ���� �˻�
		// ONLY CALL FROM INVENTORY!! ��ü ������ ���� ���������� �ϰ�ͱ� ������ �κ��丮������ ��¿ �� ���� �����´�.
	const TArray<FInventoryItem>& GetInventoryBag( EInventoryType Type ) { return m_Inventory[(int)Type]; }
	UTexture2D* GetThumbnail( int InItemID );

public:
	bool ProcessInventory( const FInventoryProcessParam& Param );
	void LoadAllTexturesInDirectory( const FString& Directory );

private:
	void LoadSpriteAsync( const FString& AssetPath );
	void UpdateInventory();
};
