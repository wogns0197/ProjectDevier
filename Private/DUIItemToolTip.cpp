#include "DUIItemToolTip.h"
#include "Components/TextBlock.h"

void UDUIItemToolTip::SetItemData( const FString& sName, const FString& sDesc )
{
	if ( TextBlock_ItemName ) {
		TextBlock_ItemName->SetText( FText::FromString( sName ) );
	}
}
