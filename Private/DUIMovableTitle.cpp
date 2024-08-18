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
	// 아무리....봐도.. 드래그로 처리하는게 맞는데 드래그는 진짜 Drag Operation이 필요한 소규모 위젯에 쓰자..

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
