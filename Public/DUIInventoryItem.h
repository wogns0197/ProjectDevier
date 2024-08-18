#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "DUIInventoryItem.generated.h"

UCLASS()
class PROJECTD_API UDUIInventoryItem : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UOverlay* Overlay_Empty;
	UPROPERTY(meta=(BindWidget))
	class UOverlay* Overlay_Full;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TextBlock_Num;
	UPROPERTY(meta=(BindWidget))
	class UImage* Image_Item;

public:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* OnMouseEnterAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* OnMouseLeaveAnim;

private:
	bool bSetData;

public:
	virtual void NativeConstruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged( bool bIsSelected ) override;
	virtual void NativeOnMouseEnter( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent );
	virtual void NativeOnMouseLeave( const FPointerEvent& InMouseEvent );
	
private:
	void SetBackgroundEmpty( bool bEmpty );
};
