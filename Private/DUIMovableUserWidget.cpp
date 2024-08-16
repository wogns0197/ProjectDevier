#include "DUIMovableUserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UDUIMovableUserWidget::SetWidgetPosition( FVector2D NewPosition )
{
    if ( UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>( ParentCanvas->GetSlots()[0]) )
    {
        CanvasSlot->SetPosition( NewPosition );
    }
}

FVector2D UDUIMovableUserWidget::GetWidgetPosition()
{
    if ( UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>( ParentCanvas->GetSlots()[0]) )
    {
        return CanvasSlot->GetPosition();
    }

    return FVector2D();
}
