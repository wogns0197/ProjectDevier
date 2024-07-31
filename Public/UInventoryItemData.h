#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UInventoryItemData.generated.h"

UCLASS()
class PROJECTD_API UUInventoryItemData : public UObject
{
	GENERATED_BODY()
	
public:
	FInventoryItem m_ItemData;
};
