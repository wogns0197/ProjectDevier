#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DUIInventory.generated.h"

enum class EInventoryType;

UCLASS()
class PROJECTD_API UDUIInventory : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

public:
	UPROPERTY(meta = (BindWidget))
	class UTileView* TileView_Inven;

private:
	EInventoryType CurInvenType;

};
