#include "DGameInstance.h"
#include "DInteractiveObject.h"
#include "DCharacter.h"
#include "CommonEnum.h"

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

bool UDGameInstance::ProcessInventory( const FInventoryParam& Param )
{
	bool bRet = false;


	return bRet;
}

