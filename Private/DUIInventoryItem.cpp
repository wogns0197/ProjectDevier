#include "DUIInventoryItem.h"
#include "../DGameInstance.h"
#include "Engine/Texture2D.h"
#include "Components/Overlay.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UInventoryItemData.h"

void UDUIInventoryItem::NativeConstruct()
{
	SetBackgroundEmpty( true );
}

void UDUIInventoryItem::NativeOnListItemObjectSet( UObject* ListItemObject )
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	UUInventoryItemData* ItemData = Cast<UUInventoryItemData>( ListItemObject );
	if ( !ItemData ) return;

	SetBackgroundEmpty( false );

	const FInventoryItem& InvenInfo = ItemData->m_ItemData;
	if ( TextBlock_Num ) {
		TextBlock_Num->SetText( FText::FromString( FString::Printf( TEXT("%d"), InvenInfo.Num ) ) );
	}

	if ( InvenInfo.ItemID != 0 && Image_Item )
	{
		// sprite atlas 사용해서 드로우콜, 로드 줄이자
		FString Path = "/Game/Content/Resources/Thumbnail/" + FString::Printf( TEXT( "%d.uasset" ), InvenInfo.ItemID );
		static ConstructorHelpers::FObjectFinder<UTexture2D> TextureAsset( *Path );
		if ( TextureAsset.Succeeded() )
		{
			UTexture2D* pTexture = TextureAsset.Object;
			FSlateBrush Brush;
			Brush.SetResourceObject( pTexture );
			Image_Item->SetBrush( Brush );
		}
	}

}

void UDUIInventoryItem::NativeOnItemSelectionChanged( bool bIsSelected )
{
}

void UDUIInventoryItem::NativeOnMouseEnter( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )
{

}

void UDUIInventoryItem::NativeOnMouseLeave( const FPointerEvent& InMouseEvent )
{
}

void UDUIInventoryItem::SetBackgroundEmpty( bool bEmpty )
{
	if ( Overlay_Empty )
		Overlay_Empty->SetVisibility( bEmpty ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed );
	if ( Overlay_Full )
		Overlay_Full->SetVisibility( !bEmpty ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed );
	if ( TextBlock_Num )
		TextBlock_Num->SetVisibility( !bEmpty ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed );
	if ( Image_Item )
		Image_Item->SetVisibility( !bEmpty ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed );
}
