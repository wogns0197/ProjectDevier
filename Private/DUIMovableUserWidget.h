#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DUIMovableUserWidget.generated.h"

UCLASS()
class UDUIMovableUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* ParentCanvas;

public:
	void SetWidgetPosition( FVector2D NewPosition );
	
};
