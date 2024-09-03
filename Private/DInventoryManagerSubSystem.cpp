#include "DInventoryManagerSubSystem.h"
#include "DInteractiveObject.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "DUIInventory.h"
#include "../DHUDBase.h"

void UDInventoryManagerSubSystem::Init( const int nCountByType, const TMap<EInventoryType, int>& mMaxCountByType )
{
	InventoryCountByType = nCountByType;
	MaxCountByBagItem = mMaxCountByType;


	m_Inventory.Init( TArray<FInventoryItem>(), 5 );
	for ( int i = 0; i < (int)EInventoryType::COUNT; ++i )
	{
		for ( int j = 0; j < InventoryCountByType; ++j )
		{
			m_Inventory[i].Emplace(FInventoryItem(0, 0));
		}
	}
}

UDInventoryManagerSubSystem* UDInventoryManagerSubSystem::GetInstance()
{
	return _instance;
}

void UDInventoryManagerSubSystem::Initialize( FSubsystemCollectionBase& Collection )
{
	Super::Initialize( Collection );
	_instance = this;
	LoadAllTexturesInDirectory( FPaths::ProjectContentDir() + "/Resources/Thumbnail/Srpite" );
}

void UDInventoryManagerSubSystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UDInventoryManagerSubSystem::IsInventoryPuttable( TWeakObjectPtr<ADInteractiveObject> InWeakPtr )
{
	if ( !InWeakPtr.IsValid() ) { return false; }

	const int nItemID = InWeakPtr.Get()->GetItemID();
	checkf( nItemID > 999, TEXT( "ItemID(%d) Is Not Valid! Actor : %s" ), nItemID, *InWeakPtr->GetClass()->GetName() );
	checkf( 1 < nItemID / 1000 && nItemID / 1000 < 5, TEXT( "Invalid Item Type | ItemID : %d" ), nItemID );

	EInventoryType InvenType = EInventoryType( nItemID / 1000 );
	bool bEmpty = false;
	for ( int i = 0; i < m_Inventory[(int)InvenType].Num(); i++ ) {
		const auto& bagItem = m_Inventory[(int)InvenType][i];
		if ( bagItem.ItemID == 0 )
		{
			// ItemID는 정상인데 Num이 0인 유령아이템 검사
			checkf( bagItem.Num < 1, TEXT( "Invalid Inventory Bag Item | Type : %d | ItemID : %d | Num : %d  << num is zero" ), (int)InvenType, nItemID, bagItem.Num );
			if ( bagItem.Num == 0 )
			{
				bEmpty = true;
				break;
			}
		}
	}

	return bEmpty;
}

bool UDInventoryManagerSubSystem::ProcessInventory( const FInventoryProcessParam& Param )
{
	bool bRet = false;
	
	if ( Param.bAdd )
	{
		bool bSameItem = false;
		FInventoryItem* TargetInventoryCell = nullptr;
		for ( auto& el : m_Inventory[(int)Param.Type] )
		{
			if ( el.ItemID == Param.ItemID && el.Num <= MaxCountByBagItem[Param.Type] ) // 같은 아이템이 이미 있으면 Max 이내에서 최대로 올려준다. 조건 분기 구조 맘에 안듬
			{
				TargetInventoryCell = &el;
				bSameItem = true;
				break;
			}
			else if ( el.ItemID == 0 && el.Num == 0 )
			{
				TargetInventoryCell = &el;
				break;
			}
		}

		if ( TargetInventoryCell )
		{
			if ( bSameItem )
			{
				TargetInventoryCell->Num = FMath::Min( TargetInventoryCell->Num + 1, MaxCountByBagItem[Param.Type] );
			}
			else
			{
				TargetInventoryCell->ItemID = Param.ItemID;
				TargetInventoryCell->Num = Param.Num;
			}
			bRet = true;
		}
	}

	else
	{
		;
	}
	
	UpdateInventory();

	return bRet;
}

void UDInventoryManagerSubSystem::UpdateInventory()
{
	ADHUDBase* hud = Cast<ADHUDBase>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if ( hud )
	{
		if ( UDUIInventory* pUI = hud->GetUI<UDUIInventory>( EUIType::UI_Inventory ) ) {
			pUI->UpdateTypeBag();
		}
	}

	// 캐릭터 포인터로 UI 및 상태 업데이트
}

void UDInventoryManagerSubSystem::LoadAllTexturesInDirectory(const FString& Directory)
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

void UDInventoryManagerSubSystem::LoadSpriteAsync(const FString& AssetPath)
{
	FStreamableManager& StreamMGR = UAssetManager::GetStreamableManager();
    FSoftObjectPath TextureAssetPath(AssetPath);

    StreamMGR.RequestAsyncLoad(TextureAssetPath, FStreamableDelegate::CreateLambda([TextureAssetPath, AssetPath, WeakPtr = TWeakObjectPtr<UDInventoryManagerSubSystem>(this)]() {
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

UTexture2D* UDInventoryManagerSubSystem::GetThumbnail( int nItemNum )
{
	if ( ThumbnailsArr.Contains( nItemNum ) )
		return ThumbnailsArr[nItemNum];

	UE_LOG( LogTemp, Warning, TEXT( "Can not Find Thumbnail ItemID : %d" ), nItemNum );
	return nullptr;
}