#include "DHUDBase.h"
#include "Blueprint/UserWidget.h"

void ADHUDBase::BeginPlay()
{
	for ( const auto& el : UISubclassMap )
	{
		UUserWidget* wnd = CreateWidget<UUserWidget>( GetWorld(), el.Value );
		if ( wnd ) {
			wnd->SetVisibility( ESlateVisibility::Collapsed );
			wnd->AddToViewport();
			UITrunk.Emplace( el.Key, wnd );
		}
	}
}

UUserWidget* ADHUDBase::OpenUI( EUIType InType )
{
	UUserWidget* wnd = UITrunk[InType];
	if ( wnd ) 
	{
		wnd->SetVisibility( ESlateVisibility::SelfHitTestInvisible );
		return wnd;
	}

	return nullptr;
}

void ADHUDBase::CloseUI( EUIType InType )
{
	if ( UUserWidget* wnd = UITrunk[InType] ) {
		wnd->SetVisibility( ESlateVisibility::Collapsed );
	}
}

bool ADHUDBase::IsUIOpened( EUIType InType )
{
	if ( UUserWidget* wnd = UITrunk[InType] ) {
		ESlateVisibility test1 = wnd->GetVisibility();
		return wnd->GetVisibility() != ESlateVisibility::Collapsed;
	}

	return false;
}