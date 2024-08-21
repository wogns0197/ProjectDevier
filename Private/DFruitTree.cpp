#include "DFruitTree.h"
#include "CommonEnum.h"
#include "DFarmCrop.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"

ADFruitTree::ADFruitTree()
{
	/*PrimaryActorTick.bCanEverTick = true;*/

	OverlapType = EOverlapObjectType::Interactive;
	InteractiveType = EInteractiveType::Trembling;
	
	if ( USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>( TEXT( "SceneComp" ) ) )
	{
		TreeSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>( TEXT( "TreeStaticMesh" ) );
		if ( TreeSkeletalMesh )
		{
			TreeSkeletalMesh->SetupAttachment( SceneComp );

			//TArray<UStaticMeshComponent*> FruitCompArr = { FruitStaticMesh_0, FruitStaticMesh_1, FruitStaticMesh_2, FruitStaticMesh_3, FruitStaticMesh_4 };
			// 아니.. 배열로 관리하면 포인터만 살고 객체 안가리킴;;;; 역시 쉽게 되는 건 없다잉...

			FruitStaticMesh_0 = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "FruitStaticMesh_0" ) );
			FruitStaticMesh_0->SetupAttachment( SceneComp );
			FruitStaticMesh_1 = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "FruitStaticMesh_1" ) );
			FruitStaticMesh_1->SetupAttachment( SceneComp );
			FruitStaticMesh_2 = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "FruitStaticMesh_2" ) );
			FruitStaticMesh_2->SetupAttachment( SceneComp );
			FruitStaticMesh_3 = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "FruitStaticMesh_3" ) );
			FruitStaticMesh_3->SetupAttachment( SceneComp );
			FruitStaticMesh_4 = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "FruitStaticMesh_4" ) );
			FruitStaticMesh_4->SetupAttachment( SceneComp );
		}

		OverlapCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>( TEXT( "OverlapComp" ) );
		if ( OverlapCapsuleComponent ) {
			OverlapCapsuleComponent->SetupAttachment( SceneComp );
		}

		BlockCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>( TEXT( "BlockComp" ) );
		if ( BlockCapsuleComponent ) {
			BlockCapsuleComponent->SetupAttachment( SceneComp );
		}
		
	}
}

void ADFruitTree::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADFruitTree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADFruitTree::OnStartInteractive()
{
	checkf( LengthGenerateFruit > 0, TEXT( "LengthGenerateFruit is Zero" ) );
	GetWorld()->GetTimerManager().SetTimer( ThrowTimeHanle, FTimerDelegate::CreateLambda( [WeakPtr = TWeakObjectPtr<ADFruitTree>( this )]() {
		if ( !WeakPtr.IsValid() ) { return; }

		const FVector GenOffset( (int)FMath::RandRange( -1, 1 ) * WeakPtr->LengthGenerateFruit, (int)FMath::RandRange( -1, 1 ) * WeakPtr->LengthGenerateFruit, WeakPtr->GetActorLocation().Z );
		WeakPtr.Get()->GetWorld()->SpawnActor( WeakPtr.Get()->FruitCropClass, &GenOffset); // 랜덤 포지션 설정해야함

	}), ThrowFruitDelayTime, false );
}

