#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DGameInstance.generated.h"

UCLASS()
class PROJECTD_API UDGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class ADCharacter* m_Character;

public:
	void Init( AActor* pOwnCharacter );	
};
