#include "DGameInstance.h"
#include "DInteractiveObject.h"
#include "Misc/Paths.h"
#include "DCharacter.h"
#include "CommonEnum.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"


void UDGameInstance::Init()
{
	Super::Init();

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

	LoadAllTexturesInDirectory( FPaths::ProjectContentDir() + "/Resources/Thumbnail/Srpite" );
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

void UDGameInstance::LoadAllTexturesInDirectory(const FString& Directory)
{
    TArray<FString> Files;
    IFileManager& FileManager = IFileManager::Get();
    FString SearchPath = Directory / TEXT("*.uasset");

    FileManager.FindFiles(Files, *SearchPath, true, false);

    for (const FString& File : Files)
    {
        FString AssetPath = Directory / File;
        AssetPath = FPackageName::FilenameToLongPackageName(AssetPath);

        LoadSpriteAsync(AssetPath);
    }

    TArray<FString> SubDirectories;
    FileManager.FindFiles(SubDirectories, *(Directory / TEXT("*")), false, true);

    for (const FString& SubDirectory : SubDirectories)
    {
        LoadAllTexturesInDirectory(Directory / SubDirectory);
    }
}

void UDGameInstance::LoadSpriteAsync(const FString& AssetPath)
{
	FStreamableManager& StreamMGR = UAssetManager::GetStreamableManager();
    FSoftObjectPath TextureAssetPath(AssetPath);

    StreamMGR.RequestAsyncLoad(TextureAssetPath, FStreamableDelegate::CreateLambda([TextureAssetPath, AssetPath, WeakPtr = TWeakObjectPtr<UDGameInstance>(this)]() {
		UTexture2D* LoadedTexture = Cast<UTexture2D>( TextureAssetPath.TryLoad() );

        if (LoadedTexture)
        {
			WeakPtr->ThumbnailsArr.Add( FCString::Atoi( *LoadedTexture->GetName() ), LoadedTexture );
            UE_LOG(LogTemp, Log, TEXT("Sprite loaded: %s"), *LoadedTexture->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to load sprite: %s"), *AssetPath);
        }
    }));
}

UTexture2D* UDGameInstance::GetThumbnail( int nItemNum )
{
	if ( ThumbnailsArr.Contains( nItemNum ) )
		return ThumbnailsArr[nItemNum];

	UE_LOG( LogTemp, Warning, TEXT( "Can not Find Thumbnail ItemID : %d" ), nItemNum );
	return nullptr;
}
