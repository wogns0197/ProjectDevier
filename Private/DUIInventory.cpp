#include "DUIInventory.h"
#include "../DGameInstance.h"
#include "UInventoryItemData.h"
#include "Components/TileView.h"
#include "Kismet/GameplayStatics.h"

void UDUIInventory::NativeConstruct()
{
}

void UDUIInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CurInvenType = EInventoryType::Equip;
	TArray<UUInventoryItemData*> ItemDataArr;

	UDGameInstance* GI = Cast<UDGameInstance>( UGameplayStatics::GetGameInstance( GetWorld() ) );
	if ( GI )
	{
		const TArray<FInventoryItem>& InvenBag = GI->GetInventoryBag( CurInvenType );
		for ( const auto& el : InvenBag )
		{
			// 메모리관리 되는거 맞는지 확인 필요
			UUInventoryItemData* ItemData = NewObject<UUInventoryItemData>();
			ItemData->SetData( el );
			ItemDataArr.Emplace( ItemData );
		}

	}

	if ( TileView_Inven ) {
		TileView_Inven->SetListItems( ItemDataArr );
	}
}
