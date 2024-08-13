#include "DUIMovableTitle.h"

void UDUIMovableTitle::NativeOnDragDetected( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation )
{
	Super::NativeOnDragDetected( InGeometry, InMouseEvent, OutOperation );
	if ( ParentWidget ) {
		const FGeometry& Geom = ParentWidget->GetCachedGeometry();
		FVector2D Position = Geom.AbsoluteToLocal(GetCachedGeometry().GetAbsolutePosition()) + GetCachedGeometry().GetLocalSize() / 2.0f;
		ParentWidget->SetPositionInViewport( Geom.AbsoluteToLocal( InMouseEvent.GetScreenSpacePosition() ) );
		int a = 0;
	}
}

FReply UDUIMovableTitle::NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        // �巡�׸� �����ϵ��� ����
        return FReply::Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
    }

    return FReply::Unhandled();
}
