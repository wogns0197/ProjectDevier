#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DUIItemToolTip.generated.h"

UCLASS()
class PROJECTD_API UDUIItemToolTip : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* TextBlock_ItemName;

public:
	void SetItemData( const FString& sName, const FString& sDesc );
};
