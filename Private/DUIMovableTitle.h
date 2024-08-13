#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DUIMovableTitle.generated.h"

// ��� Ÿ��Ʋ�� ���콺�� �巡���Ͽ� �����̴� ������ �� ������ c++,BP�� �ʼ��� �����Ͽ� ���ε��������

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
