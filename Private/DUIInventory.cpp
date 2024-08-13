#include "DUIInventory.h"
#include "../DGameInstance.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UInventoryItemData.h"
#include "DUIMovableTitle.h"
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

	if ( MovableTitle )
		MovableTitle->SetParentWidget( this );

	CurInvenType = EInventoryType::Equip;
	//test
	CurInvenType = EInventoryType::Crops;
	//test
	UpdateTypeBag();
}

FReply UDUIInventory::NativeOnKeyDown( const FGeometry& InGeometry, const FKeyEvent& InKeyEvent )
{
	TArray<FKey> CloseKeys = { EKeys::Escape, EKeys::I };
	if ( CloseKeys.Contains( InKeyEvent.GetKey() ) ) {
		if ( GetVisibility() != ESlateVisibility::Collapsed ) {
			UGameplayStatics::GetPlayerController( GetWorld(), 0 )->bShowMouseCursor = true;
			UWidgetBlueprintLibrary::SetInputMode_GameOnly( UGameplayStatics::GetPlayerController( GetWorld(), 0 ) );
		}
	}
	return FReply::Handled();
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
