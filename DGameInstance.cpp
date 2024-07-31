#include "DGameInstance.h"
#include "DInteractiveObject.h"
#include "DCharacter.h"
#include "CommonEnum.h"

void UDGameInstance::Init()
{
	m_Inventory.Init( TArray<FInventoryItem>(), 5 );
	for ( int i = 0; i < (int)EInventoryType::COUNT; ++i )
	{
		for ( int j = 0; j < InventoryCountByType; ++j )
		{
			m_Inventory[i].Emplace(FInventoryItem(0, 0));
		}
	}

	//test
	m_Inventory[2][0].ItemID = 2001;
	m_Inventory[2][0].Num = 1;
	//test
}

void UDGameInstance::InitCharacterData( ADCharacter* pOwnCharacter )
{
	if ( pOwnCharacter->IsValidLowLevel() ) {
		m_Character = Cast<ADCharacter>( pOwnCharacter );
	}
}

void UDGameInstance::ProcessInteractive( EInteractiveType InType, TWeakObjectPtr<class ADInteractiveObject> InObject )
{
	switch ( InType )
	{
	case EInteractiveType::Picking:
		if ( InObject.IsValid() )
		{
			InObject->Destroy();
			DelayedInteractDoneDelegate.Broadcast( InType );
		}
		break;
	default:
		break;
	}

	// 인벤토리 작업
}

bool UDGameInstance::ProcessInventory( const FInventoryProcessParam& Param )
{
	bool bRet = false;
	
	if ( Param.bAdd )
	{
		FInventoryItem* TargetInventoryCell = nullptr;
		for ( auto& el : m_Inventory[(int)Param.Type] )
		{
			if ( el.ItemID == 0 && el.Num == 0 ) {
				TargetInventoryCell = &el;
				break;
			}
		}

		if ( TargetInventoryCell )
		{
			TargetInventoryCell->ItemID = Param.ItemID;
			TargetInventoryCell->Num = Param.Num;
		}
	}

	else
	{
		;
	}
	
	UpdateInventory();

	return bRet;
}

void UDGameInstance::UpdateInventory()
{
	if ( m_Character )
	{
		m_Character->UpdateInventory();
	}
	// 캐릭터 포인터로 UI 및 상태 업데이트
}