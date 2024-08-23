#include "DCharacter.h"
#include "CommonEnum.h"
#include <DInteractiveObject.h>
#include "DHUDBase.h"
#include "DPlayerController.h"
#include "../DGameInstance.h"
#include "DUIInventory.h"

#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

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

	// Overlap ���� ������Ʈ
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

	auto pUI = Cast<UDUIInventory>( GetHud()->GetUI(EUIType::UI_Inventory));
	if ( !m_pUIInventory && pUI ) {
		m_pUIInventory = pUI;
	}

	for ( int i = 0; i < (int)EInteractiveType::COUNT; i++ )
	{
		m_mInteractiveArr.Add( EInteractiveType( i ), TArray<TWeakObjectPtr<ADInteractiveObject>>() );
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
	//PlayerInputComponent->BindAction( "MoveD", IE_Pressed, this, &ADCharacter::OnPressedMoveD ); // �ϴ� ���߿�...
	//PlayerInputComponent->BindAction( "MoveD", IE_Released, this, &ADCharacter::OnReleasedMoveD );
	PlayerInputComponent->BindAction("ShiftAction", IE_Pressed, this, &ADCharacter::OnShiftPressed);
    PlayerInputComponent->BindAction("ShiftAction", IE_Released, this, &ADCharacter::OnShiftReleased);
	PlayerInputComponent->BindAction("JumpAction", IE_Pressed, this, &ADCharacter::OnJumpPressed);
	PlayerInputComponent->BindAction("JumpAction", IE_Repeat, this, &ADCharacter::OnJumpRepeated);
	PlayerInputComponent->BindAction("JumpAction", IE_Released, this, &ADCharacter::OnJumpReleased);
	PlayerInputComponent->BindAction("Interactive", IE_Pressed, this, &ADCharacter::OnInteractivePressed);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &ADCharacter::OnInventoryKeyPressed);
}

void ADCharacter::LookUp( float v )
{
	AddControllerPitchInput( v );
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
	// ���ͷ�Ʈ�� ����ϴ� �Ŵ����� ���� �� ���ΰ�? �ƴϸ� ĳ���ʹܿ��� �� ó���� ���ΰ�?
	// ä��, ����, ��� �ʹ� ������ �Ŵ������� �����ϰ� ���ͷ�Ʈ �� ĳ����, ������Ʈ�� �Ѵ� ��������Ʈ�� ��ȣ �ִ°� ���� ��..
	// -> GameInstance���� ó���ϵ��� ����
	if ( WeakCurOverlapObject.IsValid() )
	{
		if ( WeakCurOverlapObject.Get()->GetOverlapType() == EOverlapObjectType::Interactive )
		{
			EInteractiveType InteractiveType = WeakCurOverlapObject.Get()->GetInteractiveType();
			CheckUnMovableState( InteractiveType );

			auto GI = Cast<UDGameInstance>( UGameplayStatics::GetGameInstance( GetWorld() ) );
			if ( WeakCurOverlapObject.IsValid() )
			{
				RotateToTarget( WeakCurOverlapObject->GetActorLocation() );
			}

			if ( InteractiveType == EInteractiveType::Picking )
			{
				bool bInvenPreRet = GI->IsInventoryPuttable( WeakCurOverlapObject );
				if ( !bInvenPreRet ) {
					// �Ұ� ä�� �޼���
					return;
				}

				Anim_bPickable = true;
				// PickObjectTime ���Ŀ� Ÿ�� ������Ʈ�� ���Ͽ� �ٵ��� �Ѵ�
				// �ӵ��� ����� ������Ʈ (ex �������� �ִ� ���� )�� �����Ҷ����� �ٷ� �տ� ���δ�
				bool bDirectAttach = WeakCurOverlapObject->GetVelocity().X > 0 || WeakCurOverlapObject->GetVelocity().Y > 0 || WeakCurOverlapObject->GetVelocity().Z > 0;
				WeakCurOverlapObject->SetPhysicsSimulate( false );
				GetWorldTimerManager().SetTimer( PickTimeHandler, FTimerDelegate::CreateLambda( [WeakThis = TWeakObjectPtr<ADCharacter>( this )]()
				{
					if ( !WeakThis.IsValid() ) { return; }

					FAttachmentTransformRules AttathRules( EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false );
					WeakThis->WeakCurOverlapObject->AttachToComponent( WeakThis->GetMesh(), AttathRules, "Fist_R_Socket" );
				} ), bDirectAttach ? 0 : PickObjectTime, false );
			}

			else if ( InteractiveType == EInteractiveType::Trembling )
			{
				bool bAbleToTrembling = GI->IsAbleToTremble( WeakCurOverlapObject );
				if ( !bAbleToTrembling ) {
					// �Ұ� ä�� �޼���
					return;
				}
				Anim_bPunching = true;
				WeakCurOverlapObject->OnStartInteractive();
			}
		}
	}
}

void ADCharacter::OnInventoryKeyPressed()
{
	// �κ��丮 UI Open or close
	UpdateInventory();
	auto hud = GetHud();
	if ( hud->IsUIOpened( EUIType::UI_Inventory ) )
	{
		SetFocusToMouseCursorAndShow( false );
		hud->CloseUI( EUIType::UI_Inventory );
	}
	else
	{
		SetFocusToMouseCursorAndShow( true );
		hud->OpenUI( EUIType::UI_Inventory );
	}
}

void ADCharacter::UpdateInventory()
{
	if ( m_pUIInventory ) {
		m_pUIInventory->UpdateTypeBag();
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
	// Ű��Ÿ �޸���� ���߿�����..�����ϴ�..

	bAbleDoublePressedRun = true;
	GetWorld()->GetTimerManager().SetTimer( DoubleMovePressedTimerHandle, FTimerDelegate::CreateLambda( [WeakPtr = TWeakObjectPtr<ADCharacter>( this )]() {
		if ( !WeakPtr.IsValid() ) { return; }
		WeakPtr.Get()->bAbleDoublePressedRun = false;
	} ), DoubleMovementTimeOffset, false );
}*/

ADInteractiveObject* ADCharacter::GetRemainInteractObject()
{
	for ( auto& el : m_mInteractiveArr ) // Enum ������ �� �켱������ ������ ������ �����ؾ��Ѵ�
	{
		if ( el.Value.Num() > 0 )
		{
			for ( const auto& elem : el.Value )
			{
				if ( elem.IsValid() )
				{
					return elem.Get();
				}
				else
				{ // valid ���ϸ� �ٷ� ı! �޸� ������ �����̴�!
					elem->MarkAsGarbage();
					el.Value.Remove( elem );
				}
			}
		}
	}

	return nullptr;
}

void ADCharacter::OnOverlapBegin( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult )
{
	// interact UI�� �켱������ �°� ������Ѵ�.
	if ( ADInteractiveObject* InteractiveObj = Cast<ADInteractiveObject>( OtherActor) )
	{
		EInteractiveType Type = InteractiveObj->GetInteractiveType();
		if ( Type == EInteractiveType::NONE ) return;

		m_mInteractiveArr[Type].Emplace( InteractiveObj );

		WeakCurOverlapObject = GetRemainInteractObject();
		if ( auto hud = GetHud() ) {
			hud->OpenUI( EUIType::UI_Interactive );
		}
	}
}

void ADCharacter::OnOverlapEnd( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex )
{
	if ( ADInteractiveObject* InteractiveObj = Cast<ADInteractiveObject>( OtherActor ) )
	{
		EInteractiveType Type = InteractiveObj->GetInteractiveType();
		if ( Type == EInteractiveType::NONE || Type == EInteractiveType::COUNT ) { return; }

		m_mInteractiveArr[Type].Remove( InteractiveObj );

		WeakCurOverlapObject = GetRemainInteractObject();
	}

	if ( WeakCurOverlapObject.IsValid() /*&& WeakCurOverlapObject->GetOverlapType() == EOverlapObjectType::Interactive */) // �ΰ� �˻簡 �ʿ��ϳ�? �̹� ������ ���������� �־��µ�
	{
		; // ��� �����
	}
	else
	{
		if ( auto hud = GetHud() ) {
			hud->CloseUI( EUIType::UI_Interactive );
		}
	}
}

void ADCharacter::OnNotifyAnimDone( EInteractiveType InType )
{
	auto GI = Cast<UDGameInstance>( UGameplayStatics::GetGameInstance( GetWorld() ) );
	GI->ProcessInteractive( InType, WeakCurOverlapObject );

	switch ( InType )
	{
	case EInteractiveType::NONE:
		break;
	case EInteractiveType::Picking:
	{
		Anim_bPickable = false;
		if ( !WeakCurOverlapObject.IsValid() ) {
			return;
		}
		break;
	}
	case EInteractiveType::Trembling:
	{
		Anim_bPunching = false;
		break;
	}
	case EInteractiveType::COUNT:
		break;
	default:
		break;
	}
}

void ADCharacter::OnInteractiveProcessDone( EInteractiveType InType )
{
	if ( !bMoveable )
	{
		switch ( InType )
		{
		case EInteractiveType::Picking:
		case EInteractiveType::Trembling:
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
		case EInteractiveType::Trembling:
		{
			// �ݱ� �߿��� ������ �� ����. ���� ����.
			bMoveable = false;
		}
			break;
		default:
			break;
	}
}

void ADCharacter::SetFocusToMouseCursorAndShow( bool bFocus )
{
	UGameplayStatics::GetPlayerController( GetWorld(), 0 )->bShowMouseCursor = bFocus;
	if ( bFocus )
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx( UGameplayStatics::GetPlayerController( GetWorld(), 0 ), nullptr, EMouseLockMode::DoNotLock );
	else
		UWidgetBlueprintLibrary::SetInputMode_GameOnly( UGameplayStatics::GetPlayerController( GetWorld(), 0 ) );
}

ADHUDBase* ADCharacter::GetHud()
{
	return Cast<ADHUDBase>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
}