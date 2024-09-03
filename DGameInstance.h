#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DGameInstance.generated.h"

enum class EInteractiveType;
// Interact Flow
// AnimNotify -> Character -> GameInstance Manage Character, InteractiveObject -> GI notify to Character, Destroy Object, Save Inventory
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FDelegateDelayedInteractDone, EInteractiveType, Type );

UCLASS()
class PROJECTD_API UDGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class ADCharacter* m_Character;

public:
	FDelegateDelayedInteractDone DelayedInteractDoneDelegate;

public:
	virtual void Init() override;
	void InitCharacterData( class ADCharacter* pOwnCharacter );
	void ProcessInteractive( EInteractiveType InType, TWeakObjectPtr<class ADInteractiveObject> InObject );
	bool IsAbleToTremble( TWeakObjectPtr<class ADInteractiveObject> InWeakPtr );
};
