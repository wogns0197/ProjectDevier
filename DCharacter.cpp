#include "DCharacter.h"
#include "CommonEnum.h"
#include <DInteractiveObject.h>
#include "DHUDBase.h"
#include "DPlayerController.h"
#include "../DGameInstance.h"

#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ADCharacter::ADCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>( TEXT( "SpringArm" ) );
	if ( SpringArmComponent )
	{
		SpringArmComponent->SetupAttachment( RootComponent );
		CameraComponent = CreateDefaultSubobject<UCameraComponent>( TEXT( "Camera" ) );
		SpringArmComponent->bUsePawnControlRotation = true;
		if ( CameraComponent )
		{
			CameraComponent->SetupAttachment( SpringArmComponent );
		}
	}

	// Overlap 관리 컴포넌트
	OverlapCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>( TEXT( "OverlapComp" ) );
	if ( OverlapCapsuleComponent )
	{
		OverlapCapsuleComponent->SetupAttachment( RootComponent );
		OverlapCapsuleComponent->SetCollisionEnabled( ECollisionEnabled::QueryOnly );
		OverlapCapsuleComponent->SetCollisionObjectType( ECollisionChannel::ECC_Pawn );
		OverlapCapsuleComponent->SetCollisionResponseToAllChannels( ECollisionResponse::ECR_Overlap );
		OverlapCapsuleComponent->OnComponentBeginOverlap.AddDynamic( this, &ADCharacter::OnOverlapBegin );
		OverlapCapsuleComponent->OnComponentEndOverlap.AddDynamic( this, &ADCharacter::OnOverlapEnd );
	}
	
}

void ADCharacter::BeginPlay()
{
	Super::BeginPlay();
	bAbleDoublePressedRun = false;
	bMoveable = true;
	dBaseWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	AnimInstance = Cast<UAnimInstance>( GetMesh()->GetAnimClass() );
	GetWorldTimerManager().ClearTimer( PickTimeHandler );

	if ( auto gi = Cast<UDGameInstance>( UGameplayStatics::GetGameInstance( GetWorld() ) ) )
	{
		gi->InitCharacterData( this );
		gi->DelayedInteractDoneDelegate.AddDynamic( this, &ADCharacter::OnInteractiveProcessDone );
	}
}

void ADCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ( SpringArmComponent )
	{
		SpringArmComponent->TargetArmLength = FMath::FInterpTo(SpringArmComponent->TargetArmLength, dCurArmLength, DeltaTime, ZoomSpeed);
	}
}

void ADCharacter::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

	PlayerInputComponent->BindAxis( "Lookup", this, &APawn::AddControllerPitchInput );
	PlayerInputComponent->BindAxis( "Turn", this, &APawn::AddControllerYawInput );
	PlayerInputComponent->BindAxis( "MoveForward", this, &ADCharacter::MoveForward );
	PlayerInputComponent->BindAxis( "MoveRight", this, &ADCharacter::MoveRight );
	PlayerInputComponent->BindAxis( "MouseWheel", this, &ADCharacter::OnMouseWheel );
	//PlayerInputComponent->BindAction( "MoveD", IE_Pressed, this, &ADCharacter::OnPressedMoveD ); // 일단 나중에...
	//PlayerInputComponent->BindAction( "MoveD", IE_Released, this, &ADCharacter::OnReleasedMoveD );
	PlayerInputComponent->BindAction("ShiftAction", IE_Pressed, this, &ADCharacter::OnShiftPressed);
    PlayerInputComponent->BindAction("ShiftAction", IE_Released, this, &ADCharacter::OnShiftReleased);
	PlayerInputComponent->BindAction("JumpAction", IE_Pressed, this, &ADCharacter::OnJumpPressed);
	PlayerInputComponent->BindAction("JumpAction", IE_Repeat, this, &ADCharacter::OnJumpRepeated);
	PlayerInputComponent->BindAction("JumpAction", IE_Released, this, &ADCharacter::OnJumpReleased);
	PlayerInputComponent->BindAction("Interactive", IE_Pressed, this, &ADCharacter::OnInteractivePressed);
}

void ADCharacter::LookUp( float v )
{
	AddControllerPitchInput( -v );
}

void ADCharacter::Turn( float v )
{
	AddControllerYawInput( v );
}

void ADCharacter::MoveForward( float v )
{
	if ( v != 0.f && bMoveable )
	{
		FVector ForwardDir = CameraComponent->GetForwardVector();
		ForwardDir.Z = 0;
		ForwardDir.Normalize();
		AddMovementInput( ForwardDir, v );
		RotateToDirection( ForwardDir * v );
	}
}

void ADCharacter::MoveRight( float v )
{
	if ( v != 0.f && bMoveable )
	{
		FVector RightDir = CameraComponent->GetRightVector();
		RightDir.Z = 0;
		RightDir.Normalize();
		AddMovementInput( RightDir, v );
		RotateToDirection( RightDir * v );
	}
}

void ADCharacter::OnMouseWheel( float v )
{
	dCurArmLength = FMath::Clamp( dCurArmLength + (-v * ZoomDelta), MinArmLength, MaxArmLength );
}

void ADCharacter::OnShiftPressed()
{
	if ( !bPressingShift )
	{
		bPressingShift = true;
		GetCharacterMovement()->MaxWalkSpeed *= RunSpeedOffset;
	}
}

void ADCharacter::OnShiftReleased()
{
	if ( bPressingShift )
	{
		bPressingShift = false;
		GetCharacterMovement()->MaxWalkSpeed /= RunSpeedOffset;
	}
}

void ADCharacter::OnJumpPressed()
{
	if ( !GetMovementComponent()->IsFlying() && !GetCharacterMovement()->IsFalling() ) {
		bPressedJump = true;
	}
}

void ADCharacter::OnJumpRepeated()
{
	if ( !GetMovementComponent()->IsFlying() && !GetCharacterMovement()->IsFalling() ) {
		bPressedJump = true;
	}
}

void ADCharacter::OnJumpReleased()
{
	bPressedJump = false;
}

void ADCharacter::OnInteractivePressed()
{
	// 인터랙트를 담당하는 매니저를 따로 둘 것인가? 아니면 캐릭터단에서 다 처리할 것인가?
	// 채집, 낚시, 등등 너무 많은데 매니저에서 관리하고 인터랙트 시 캐릭터, 오브젝트에 둘다 델리게이트로 신호 주는게 나을 듯..
	// -> GameInstance에서 처리하도록 하자
	if ( WeakCurOverlapObject.IsValid() )
	{
		if ( WeakCurOverlapObject.Get()->GetOverlapType() == EOverlapObjectType::Interactive )
		{
			EInteractiveType InteractiveType = WeakCurOverlapObject.Get()->GetInteractiveType();
			CheckUnMovableState( InteractiveType );

			if ( WeakCurOverlapObject.IsValid() )
			{
				RotateToTarget( WeakCurOverlapObject->GetActorLocation() );
			}

			if ( InteractiveType == EInteractiveType::Picking )
			{
				Anim_bPickable = true;
				// PickObjectTime 이후에 타겟 오브젝트가 소켓에 붙도록 한다
				GetWorldTimerManager().SetTimer( PickTimeHandler, FTimerDelegate::CreateLambda( [WeakThis = TWeakObjectPtr<ADCharacter>( this ) ]()
				{
					if ( !WeakThis.IsValid() ) { return; }

					FAttachmentTransformRules AttathRules( EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false );
					WeakThis.Get()->WeakCurOverlapObject->AttachToComponent( WeakThis.Get()->GetMesh(), AttathRules, "Fist_R_Socket" );
				}), PickObjectTime, false);
			}
		}
	}
}

/*void ADCharacter::OnPressedMoveD()
{
	if ( bAbleDoublePressedRun )
	{
		OnShiftPressed();

		if ( GetWorldTimerManager().IsTimerActive( DoubleMovePressedTimerHandle ) ) {
			GetWorldTimerManager().ClearTimer( DoubleMovePressedTimerHandle );
		}
	}
}*/

/*void ADCharacter::OnReleasedMoveD()
{
	// 키연타 달리기는 나중에하자..복잡하다..

	bAbleDoublePressedRun = true;
	GetWorld()->GetTimerManager().SetTimer( DoubleMovePressedTimerHandle, FTimerDelegate::CreateLambda( [WeakPtr = TWeakObjectPtr<ADCharacter>( this )]() {
		if ( !WeakPtr.IsValid() ) { return; }
		WeakPtr.Get()->bAbleDoublePressedRun = false;
	} ), DoubleMovementTimeOffset, false );
}*/

void ADCharacter::OnOverlapBegin( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult )
{
	if ( ADInteractiveObject* InteractiveObj = Cast<ADInteractiveObject>( OtherActor) ) {
		WeakCurOverlapObject = InteractiveObj;
		if ( auto hud = GetHud() ) {
			hud->OpenUI( EUIType::UI_Interactive );
		}
	}
}

void ADCharacter::OnOverlapEnd( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex )
{
	if ( WeakCurOverlapObject == OtherActor )
	{
		if ( WeakCurOverlapObject.IsValid() && WeakCurOverlapObject->GetOverlapType() == EOverlapObjectType::Interactive )
		{
			if ( auto hud = GetHud() ) {
				hud->CloseUI( EUIType::UI_Interactive );
			}
		}

		WeakCurOverlapObject = nullptr;
	}
}

void ADCharacter::OnNotifyAnimDone( EInteractiveType InType )
{
	if ( InType == EInteractiveType::Picking )
	{
		if ( auto GI = Cast<UDGameInstance>( UGameplayStatics::GetGameInstance( GetWorld() ) ) )
		{
			Anim_bPickable = false;
			if ( !WeakCurOverlapObject.IsValid() ) {
				return;
			}

			GI->ProcessInteractive( EInteractiveType::Picking, WeakCurOverlapObject );
		}
	}
}

void ADCharacter::OnInteractiveProcessDone( EInteractiveType InType )
{
	if ( !bMoveable )
	{
		switch ( InType )
		{
		case EInteractiveType::Picking:
			bMoveable = true;
			break;
		default:
			break;
		}
	}
}
void ADCharacter::RotateToTarget( FVector vec )
{
	FRotator NewRot = UKismetMathLibrary::FindLookAtRotation( GetActorLocation(), vec );
	NewRot.Pitch = 0.f;
	NewRot.Roll = 0.f;
	SetActorRotation( NewRot );
}

void ADCharacter::RotateToDirection( FVector RotVec )
{
	if ( RotVec.SizeSquared() > 0 )
	{
		FRotator TargetRot = RotVec.Rotation();

		FRotator CurRot = GetActorRotation();
		FRotator NewRot = FQuat::Slerp( FQuat( CurRot ), FQuat( TargetRot ), RotSpeed * GetWorld()->GetDeltaSeconds() ).Rotator();

		SetActorRotation( NewRot );
	}
}

void ADCharacter::CheckUnMovableState( EInteractiveType InteractiveType )
{
	switch ( InteractiveType )
	{
		case EInteractiveType::Picking:
		{
			// 줍기 중에는 움직일 수 없다.
			bMoveable = false;
		}
			break;
		default:
			break;
	}
}

ADHUDBase* ADCharacter::GetHud()
{
	return Cast<ADHUDBase>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
}