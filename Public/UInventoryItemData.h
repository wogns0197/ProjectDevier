#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../DGameInstance.h"
#include "UInventoryItemData.generated.h"


UCLASS()
class PROJECTD_API UUInventoryItemData : public UObject
{
	GENERATED_BODY()
	
public:
	// 일부러 const&를 안쓰고 복사로 넣어준다
	void SetData( FInventoryItem InData ) { m_ItemData = InData; }

public:
	FInventoryItem m_ItemData;
};
