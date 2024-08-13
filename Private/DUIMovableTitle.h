#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DUIMovableTitle.generated.h"

// 상단 타이틀을 마우스로 드래그하여 움직이는 위젯은 이 위젯을 c++,BP에 필수로 저장하여 바인딩해줘야함

UCLASS()
class UDUIMovableTitle : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UUserWidget* ParentWidget;

public:
	void SetParentWidget( UUserWidget* wnd ) { ParentWidget = wnd; }

public:
	virtual void NativeOnDragDetected( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation ) override;
	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
};
