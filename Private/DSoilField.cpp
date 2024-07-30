#include "DSoilField.h"
#include "DSoilField.h"
#include "DFarmCrop.h"
#include "Components/StaticMeshComponent.h"

ADSoilField::ADSoilField()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>( TEXT( "SceneComp" ) );
	if ( SceneComp )
	{
		StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "StaticMeshComp" ) );
		if ( StaticMesh )
		{
			StaticMesh->SetupAttachment( SceneComp );
		}
	}
}

void ADSoilField::BeginPlay()
{
	Super::BeginPlay();
	
	InitCrops();
}

void ADSoilField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADSoilField::InitCrops()
{
	if ( !BP_FarmCrop || !intervalX || !intervalY ) {
		return;
	}

	FVector origin, Bounds;
	GetActorBounds( false, origin, Bounds );
	origin = GetActorLocation();
	
	if ( StaticMesh ) {
		UE_LOG( LogTemp, Warning, TEXT( "origin = %d, %d | Bounds = %d, %d" ),
			origin.X, origin.Y, Bounds.X, Bounds.Y );
	}

	/*Bounds.X /= 1.5f;
	Bounds.Y /= 1.5f;*/
	TArray<FVector> CropSpawnPtArr;

	// 원형 소환이 뭔가 이상해....분명 타원이 아닌데... 아무튼 이상해서 정사각형 격자로 간다...
	/*int Deg = 0;
	for ( ; Deg < 360; Deg += (360 / SpawnDivNum) )
	{
		FVector pt;
		pt.X = origin.X + (FMath::Cos( Deg ) * Bounds.X);
		pt.Y = origin.Y + (FMath::Sin( Deg ) * Bounds.X);
		pt.Z = Bounds.Z / 3;
		CropSpawnPtArr.Emplace( pt );
	}*/

	for ( int i = origin.X - Bounds.X / 2; i < origin.X + Bounds.X / 2; i += intervalX )
	{
		for ( int j = origin.Y - Bounds.Y / 2; j < origin.Y + Bounds.Y / 2; j += intervalY )
		{
			CropSpawnPtArr.Emplace( FVector( i, j, origin.Z ) );
		}
	}


	for ( const auto& el : CropSpawnPtArr ) {
		GetWorld()->SpawnActor<ADFarmCrop>( BP_FarmCrop, el, FRotator(0) );
	}
}

