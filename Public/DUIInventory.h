#pragma once

#include "CoreMinimal.h"
#include "DUIMovableUserWidget.h"
#include "DUIInventory.generated.h"

enum class EInventoryType;

UCLASS()
class PROJECTD_API UDUIInventory : public UDUIMovableUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnKeyDown( const FGeometry& InGeometry, const FKeyEvent& InKeyEvent ) override;

	UFUNCTION()
	void OnVisibilityChanged_Callback(ESlateVisibility vis );

	void UpdateTypeBag();

public:
	UPROPERTY(meta = (BindWidget))
	class UTileView* TileView_Inven;
	UPROPERTY(meta = (BindWidget))
	class UDUIMovableTitle* MovableTitle;

private:
	EInventoryType CurInvenType;
};
