#include "DUIInventoryItem.h"
#include "Engine/Texture2D.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Overlay.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UInventoryItemData.h"
#include "DInventoryManagerSubSystem.h"
#include "../DHUDBase.h"

void UDUIInventoryItem::NativeConstruct()
{
	SetBackgroundEmpty( true );
	bSetData = false;
}

void UDUIInventoryItem::NativeOnListItemObjectSet( UObject* ListItemObject )
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	SetBackgroundEmpty( true );

	UUInventoryItemData* ItemData = Cast<UUInventoryItemData>( ListItemObject );
	if ( !ItemData ) return;

	const FInventoryItem& InvenInfo = ItemData->m_ItemData;
	if ( TextBlock_Num ) {
		TextBlock_Num->SetText( FText::FromString( FString::Printf( TEXT("%d"), InvenInfo.Num ) ) );
	}

	if ( InvenInfo.ItemID != 0 && Image_Item )
	{
		SetBackgroundEmpty( false );
		// sprite atlas 사용해서 드로우콜, 로드 줄이자
		auto InvenMgr = UDInventoryManagerSubSystem::GetInstance();

		FSlateBrush Brush;
		Brush.SetResourceObject( InvenMgr->GetThumbnail( InvenInfo.ItemID ) );
		Image_Item->SetBrush( Brush );
		bSetData = true;
		/*if ( TextureAsset.Succeeded() )
		{
			UTexture2D* pTexture = TextureAsset.Object;
			FSlateBrush Brush;
			Brush.SetResourceObject( pTexture );
			Image_Item->SetBrush( Brush );
		}*/
	}

}

void UDUIInventoryItem::NativeOnItemSelectionChanged( bool bIsSelected )
{
}

void UDUIInventoryItem::NativeOnMouseEnter( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )
{
	if ( bSetData )
	{
		if ( Image_Item && OnMouseEnterAnim )
			PlayAnimation( OnMouseEnterAnim );
		UGameplayStatics::GetPlayerController( GetWorld(), 0 )->CurrentMouseCursor = EMouseCursor::GrabHand;

		if ( auto pHud = Cast<ADHUDBase>( UGameplayStatics::GetPlayerController( this, 0 )->GetHUD() ) )
		{
			FGeometry geom = GetCachedGeometry();
			auto pUIToolTip = pHud->OpenUI( EUIType::UI_ToolTip, FVector2D(0));
		}
	}
}

void UDUIInventoryItem::NativeOnMouseLeave( const FPointerEvent& InMouseEvent )
{
	if ( bSetData )
	{
		if ( Image_Item && OnMouseLeaveAnim )
			PlayAnimation( OnMouseLeaveAnim );
		UGameplayStatics::GetPlayerController( GetWorld(), 0 )->CurrentMouseCursor = EMouseCursor::Default;

		if ( auto pHud = Cast<ADHUDBase>( UGameplayStatics::GetPlayerController( this, 0 )->GetHUD() ) ) {
			pHud->CloseUI( EUIType::UI_ToolTip );
		}
	}
}

FReply UDUIInventoryItem::NativeOnMouseButtonDoubleClick( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )
{
	if ( !bSetData ) return FReply::Unhandled();
	FReply ret = Super::NativeOnMouseButtonDoubleClick( InGeometry, InMouseEvent );

	return ret;

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
