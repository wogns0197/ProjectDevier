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
	UPROPERTY(meta = (BindWidget))
	class UTileView* TileView_Inven;
	UPROPERTY(meta = (BindWidget))
	class UDUIMovableTitle* MovableTitle;
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Equip;
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Consume;
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Crop;
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Etc;
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Cash;

	UPROPERTY(EditAnywhere)
	FLinearColor DefulatButtonColor;
	UPROPERTY(EditAnywhere)
	FLinearColor SelectButtonColor;

private:
	EInventoryType CurInvenType;
	TArray<class UButton*> TypeButtonArr;

public:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnKeyDown( const FGeometry& InGeometry, const FKeyEvent& InKeyEvent ) override;

	UFUNCTION()
	void OnVisibilityChanged_Callback(ESlateVisibility vis );
	UFUNCTION()
	void OnClicked_Equip();
	UFUNCTION()
	void OnClicked_Consume();
	UFUNCTION()
	void OnClicked_Crop();
	UFUNCTION()
	void OnClicked_Etc();
	UFUNCTION()
	void OnClicked_Cash();

public:
	void UpdateTypeBag();

private:
	void ChangeBagTypeButton( EInventoryType InType );

};
