#include "DUIInventory.h"
#include "../DGameInstance.h"
#include "UInventoryItemData.h"
#include "Components/TileView.h"
#include "Kismet/GameplayStatics.h"

void UDUIInventory::NativeConstruct()
{
	if ( !OnVisibilityChanged.IsBound() )
		OnVisibilityChanged.AddDynamic( this, &UDUIInventory::OnVisibilityChanged_Callback );
}

void UDUIInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CurInvenType = EInventoryType::Equip;
	//test
	CurInvenType = EInventoryType::Crops;
	//test
	UpdateTypeBag();
}

void UDUIInventory::OnVisibilityChanged_Callback( ESlateVisibility vis )
{
	if ( vis != ESlateVisibility::Collapsed )
		SetFocus();
}

void UDUIInventory::UpdateTypeBag()
{
	if ( !TileView_Inven ) {
		return;
	}

	TileView_Inven->ClearListItems();

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

	TileView_Inven->RegenerateAllEntries();
	TileView_Inven->SetListItems( ItemDataArr );
}
