#include "DUIMovableTitle.h"
#include "DUIMovableUserWidget.h"
#include "Blueprint/SlateBlueprintLibrary.h"

FReply UDUIMovableTitle::NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )
{
	Super::NativeOnMouseButtonDown( InGeometry, InMouseEvent );

	bMouseDown = true;

	if ( ParentWidget )
		{
			const FGeometry& Geom = ParentWidget->GetCachedGeometry();
			Pivot = Geom.AbsoluteToLocal( InMouseEvent.GetScreenSpacePosition() ) - ParentWidget->GetWidgetPosition();
		}

	return FReply::Handled();
}

FReply UDUIMovableTitle::NativeOnMouseButtonUp( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )
{
	Super::NativeOnMouseButtonUp( InGeometry, InMouseEvent );

	bMouseDown = false;

	return FReply::Handled();
}

void UDUIMovableTitle::SetParentWidget( UUserWidget* wnd )
{ 
	if ( UDUIMovableUserWidget* MovableWidget = Cast<UDUIMovableUserWidget>( wnd ) ) {
		ParentWidget = MovableWidget;
	}
}

void UDUIMovableTitle::NativeOnMouseEnter( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )
{
	Super::NativeOnMouseEnter( InGeometry, InMouseEvent );

	bMouseDown = false;
	bMouseIn = true;
}

void UDUIMovableTitle::NativeOnMouseLeave( const FPointerEvent& InMouseEvent )
{
	Super::NativeOnMouseLeave( InMouseEvent );

	bMouseIn = false;
}

FReply UDUIMovableTitle::NativeOnMouseMove( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )
{
	// �ƹ���....����.. �巡�׷� ó���ϴ°� �´µ� �巡�״� ��¥ Drag Operation�� �ʿ��� �ұԸ� ������ ����..

	if ( bMouseDown && bMouseIn )
	{
		if ( ParentWidget )
		{
			const FGeometry& Geom = ParentWidget->GetCachedGeometry();
			ParentWidget->SetWidgetPosition( Geom.AbsoluteToLocal( InMouseEvent.GetScreenSpacePosition() ) - Pivot );
		}
	}

	return Super::NativeOnMouseMove( InGeometry, InMouseEvent );
}
