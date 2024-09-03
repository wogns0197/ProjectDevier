#include "DUIInventory.h"
#include "../DGameInstance.h"
#include "DInventoryManagerSubSystem.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UInventoryItemData.h"
#include "DUIMovableTitle.h"
#include "Components/Button.h"
#include "Components/TileView.h"
#include "Kismet/GameplayStatics.h"

void UDUIInventory::NativeConstruct()
{
	if ( !OnVisibilityChanged.IsBound() )
		OnVisibilityChanged.AddDynamic( this, &UDUIInventory::OnVisibilityChanged_Callback );

	if ( Button_Equip && !Button_Equip->OnClicked.IsBound() )
		Button_Equip->OnClicked.AddDynamic( this, &UDUIInventory::OnClicked_Equip );
	if ( Button_Consume && !Button_Consume->OnClicked.IsBound() )
		Button_Consume->OnClicked.AddDynamic( this, &UDUIInventory::OnClicked_Consume );
	if ( Button_Crop && !Button_Crop->OnClicked.IsBound() )
		Button_Crop->OnClicked.AddDynamic( this, &UDUIInventory::OnClicked_Crop );
	if ( Button_Etc && !Button_Etc->OnClicked.IsBound() )
		Button_Etc->OnClicked.AddDynamic( this, &UDUIInventory::OnClicked_Etc );
	if ( Button_Cash && !Button_Cash->OnClicked.IsBound() )
		Button_Cash->OnClicked.AddDynamic( this, &UDUIInventory::OnClicked_Cash );

	// 나중에 ini 설정데이터에서 읽자
	CurInvenType = EInventoryType::Equip;
}

void UDUIInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TypeButtonArr = { Button_Equip, Button_Consume, Button_Crop, Button_Etc, Button_Cash };

	if ( MovableTitle )
		MovableTitle->SetParentWidget( this );

	UpdateTypeBag();
}

FReply UDUIInventory::NativeOnKeyDown( const FGeometry& InGeometry, const FKeyEvent& InKeyEvent )
{
	TArray<FKey> CloseKeys = { EKeys::Escape, EKeys::I };
	if ( CloseKeys.Contains( InKeyEvent.GetKey() ) ) {
		if ( GetVisibility() != ESlateVisibility::Collapsed )
		{
			UGameplayStatics::GetPlayerController( GetWorld(), 0 )->bShowMouseCursor = false;
			UWidgetBlueprintLibrary::SetInputMode_GameOnly( UGameplayStatics::GetPlayerController( GetWorld(), 0 ) );
			SetVisibility( ESlateVisibility::Collapsed );
		}
	}
	return FReply::Handled();
}

void UDUIInventory::OnVisibilityChanged_Callback( ESlateVisibility vis )
{
	if ( vis != ESlateVisibility::Collapsed )
		SetFocus();

	if ( UDInventoryManagerSubSystem* InvenMgr = UDInventoryManagerSubSystem::GetInstance() ) {
		InvenMgr->LastInventoryUIBagType = CurInvenType;
	}
}

void UDUIInventory::OnClicked_Equip()
{
	if ( CurInvenType == EInventoryType::Equip ) { return; }
	CurInvenType = EInventoryType::Equip;
	UpdateTypeBag();
}

void UDUIInventory::OnClicked_Consume()
{
	if ( CurInvenType == EInventoryType::Consume ) { return; }
	CurInvenType = EInventoryType::Consume;
	UpdateTypeBag();
}

void UDUIInventory::OnClicked_Crop()
{
	if ( CurInvenType == EInventoryType::Crops ) { return; }
	CurInvenType = EInventoryType::Crops;
	UpdateTypeBag();
}

void UDUIInventory::OnClicked_Etc()
{
	if ( CurInvenType == EInventoryType::Etc ) { return; }
	CurInvenType = EInventoryType::Etc;
	UpdateTypeBag();
}

void UDUIInventory::OnClicked_Cash()
{
	if ( CurInvenType == EInventoryType::Cash ) { return; }
	CurInvenType = EInventoryType::Cash;
	UpdateTypeBag();
}

void UDUIInventory::UpdateTypeBag()
{
	ChangeBagTypeButton( CurInvenType );

	if ( !TileView_Inven ) {
		return;
	}

	TileView_Inven->ClearListItems();

	TArray<UUInventoryItemData*> ItemDataArr;
	if ( UDInventoryManagerSubSystem* InvenMgr = UDInventoryManagerSubSystem::GetInstance() )
	{
		const TArray<FInventoryItem>& InvenBag = InvenMgr->GetInventoryBag( CurInvenType );
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

void UDUIInventory::ChangeBagTypeButton( EInventoryType InType )
{
	CurInvenType = InType;
	for ( const auto& el : TypeButtonArr ) {
		FSlateBrush NormalBrush = el->WidgetStyle.Normal;
		NormalBrush.TintColor = DefulatButtonColor;
		el->WidgetStyle.SetNormal( NormalBrush );
	}

	if ( auto TargetButton = TypeButtonArr[(int)CurInvenType] ) {
		FSlateBrush NormalBrush = TargetButton->WidgetStyle.Normal;
		NormalBrush.TintColor = SelectButtonColor;
		TargetButton->WidgetStyle.SetNormal( NormalBrush );
	}
}
