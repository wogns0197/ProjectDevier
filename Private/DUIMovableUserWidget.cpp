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