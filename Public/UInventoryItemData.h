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
	// �Ϻη� const&�� �Ⱦ��� ����� �־��ش�
	void SetData( FInventoryItem InData ) { m_ItemData = InData; }

public:
	FInventoryItem m_ItemData;
};
