#include "DGameInstance.h"
#include "DInteractiveObject.h"
#include "Misc/Paths.h"
#include "DCharacter.h"
#include "DInventoryManagerSubSystem.h"
#include "CommonEnum.h"


void UDGameInstance::Init()
{
	Super::Init();

	auto pInvenMgr = UDInventoryManagerSubSystem::GetInstance();
	if ( pInvenMgr )
		pInvenMgr->Init( InventoryCountByType, MaxCountByBagItem );
}

void UDGameInstance::InitCharacterData( ADCharacter* pOwnCharacter )
{
	if ( pOwnCharacter->IsValidLowLevel() ) {
		m_Character = Cast<ADCharacter>( pOwnCharacter );
	}
}

bool UDGameInstance::IsAbleToTremble( TWeakObjectPtr<class ADInteractiveObject> InWeakPtr )
{
	bool bRet = false;
	if ( InWeakPtr.IsValid() )
	{
		if ( InWeakPtr->GetInteractiveType() == EInteractiveType::Trembling ) {
			bRet = true;
		}
	}
		
	return bRet;
}

// 습득 처리 순서
// OnInteractivePressed -> IsInventoryPuttable -> RotateToTarget -> ..Timer..AttachToComponent -> Anim_bPickable ( Play Animation ) -> AnimNotify to GI -> Fill Inven -> Destroy
void UDGameInstance::ProcessInteractive( EInteractiveType InType, TWeakObjectPtr<class ADInteractiveObject> InObject )
{
	switch ( InType )
	{
	case EInteractiveType::Picking:
	{
		if ( InObject.IsValid() )
		{
			// 여기서 기본 갯수를 1로 하는데, 나중에 복수로 바꾸려면 인벤에 개별가방 칸당 복수처리가 되어야함 아래 코드가 굉장히 별로임
			const int nItemID = InObject.Get()->GetItemID();
			FInventoryProcessParam Item( EInventoryType( nItemID / 1000 ), true, nItemID, 1, 0 );
			if ( UDInventoryManagerSubSystem* InvenMgr = UDInventoryManagerSubSystem::GetInstance() )
			{
				if ( !InvenMgr->ProcessInventory( Item ) ) {
				UE_LOG( LogTemp, Warning, TEXT( "ProcessInventory Fail :: ItemID|%d|ActorClass|%s" ), nItemID, *InObject->GetClass()->GetName() );
			}
			}

			InObject->Destroy();
		}
		break;
	}
	case EInteractiveType::Trembling:
	{
		// 나무 흔들기는 Obj Valid 안중요할 것 같아서 따로 안넣는다
		// @TODO..
	}
	default:
		break;
	}

	DelayedInteractDoneDelegate.Broadcast( InType );

	// 인벤토리 작업
}
