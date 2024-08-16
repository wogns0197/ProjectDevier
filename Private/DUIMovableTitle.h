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
	class UDUIMovableUserWidget* ParentWidget;
	bool bMouseIn = false;
	bool bMouseDown = false;

public:
	void SetParentWidget( UUserWidget* wnd );

public:
	virtual void NativeOnMouseEnter( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual void NativeOnMouseLeave( const FPointerEvent& InMouseEvent ) override;
	virtual FReply NativeOnMouseMove( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual FReply NativeOnMouseButtonUp( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
};
