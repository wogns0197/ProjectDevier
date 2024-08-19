#include "DFruitTree.h"
#include "CommonEnum.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"

ADFruitTree::ADFruitTree()
{
	/*PrimaryActorTick.bCanEverTick = true;*/

	OverlapType = EOverlapObjectType::Interactive;
	InteractiveType = EInteractiveType::Trembling;
	
	if ( USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>( TEXT( "SceneComp" ) ) )
	{
		TreeStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "TreeStaticMesh" ) );
		if ( TreeStaticMesh )
		{
			TreeStaticMesh->SetupAttachment( SceneComp );

			//TArray<UStaticMeshComponent*> FruitCompArr = { FruitStaticMesh_0, FruitStaticMesh_1, FruitStaticMesh_2, FruitStaticMesh_3, FruitStaticMesh_4 };
			// �ƴ�.. �迭�� �����ϸ� �����͸� ��� ��ü �Ȱ���Ŵ;;;; ���� ���� �Ǵ� �� ������...

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

