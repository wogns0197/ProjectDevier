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

// ���� ó�� ����
// OnInteractivePressed -> IsInventoryPuttable -> RotateToTarget -> ..Timer..AttachToComponent -> Anim_bPickable ( Play Animation ) -> AnimNotify to GI -> Fill Inven -> Destroy
void UDGameInstance::ProcessInteractive( EInteractiveType InType, TWeakObjectPtr<class ADInteractiveObject> InObject )
{
	switch ( InType )
	{
	case EInteractiveType::Picking:
	{
		if ( InObject.IsValid() )
		{
			// ���⼭ �⺻ ������ 1�� �ϴµ�, ���߿� ������ �ٲٷ��� �κ��� �������� ĭ�� ����ó���� �Ǿ���� �Ʒ� �ڵ尡 ������ ������
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
		// ���� ����� Obj Valid ���߿��� �� ���Ƽ� ���� �ȳִ´�
		// @TODO..
	}
	default:
		break;
	}

	DelayedInteractDoneDelegate.Broadcast( InType );

	// �κ��丮 �۾�
}
