#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DHUDBase.generated.h"

enum class EOverlapObjectType;

DECLARE_DYNAMIC_DELEGATE_OneParam( FDelegateInteractiveOverlapBegin, EOverlapObjectType, Type );
DECLARE_DYNAMIC_DELEGATE_OneParam( FDelegateInteractiveOverlapEnd, EOverlapObjectType, Type );
// 오버랩이 중복될 것을 고려하여 오버랩 해제시에도 타입을 받아서 타입별 처리하도록 추가

UENUM( BlueprintType )
enum class EUIType : uint8
{
	NONE,
	UI_Interactive,
	UI_Inventory,
	UI_Hud,
	UI_Map,
	UI_VandingMachine,
};

UCLASS()
class PROJECTD_API ADHUDBase : public AHUD
{
	GENERATED_BODY()
public:
	FDelegateInteractiveOverlapBegin OnInteractiveOvelapBegin;
	FDelegateInteractiveOverlapEnd OnInteractiveOvelapEnd;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere)
	TMap<EUIType, TSubclassOf<UUserWidget>> UISubclassMap;
	UPROPERTY()
	TMap<EUIType, UUserWidget*> UITrunk;
		
public:
	UUserWidget* GetUI( EUIType InType ) { return UITrunk[InType]; }
	UUserWidget* OpenUI( EUIType InType );
	bool IsUIOpened( EUIType InType );
	void CloseUI( EUIType InType );
};

