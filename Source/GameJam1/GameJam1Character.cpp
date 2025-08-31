// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameJam1Character.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PlayerUI.h"
#include "GameManager.h"
#include "Blueprint/UserWidget.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AGameJam1Character

AGameJam1Character::AGameJam1Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 900.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 50.0f;


	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	m_Health = m_MaxHealth;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGameJam1Character::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AGameJam1Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGameJam1Character::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGameJam1Character::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AGameJam1Character::Die()
{
	GetMesh()->PlayAnimation(m_DeathAnim, false);
	//GetCharacterMovement()->DisableMovement();
	LockMoveInput(true);
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_playerUI->UpdateHealthUI(m_Health);
	TArray<AActor*> found;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Manager"), found);
	if (found.Num() == 0) return;

	if (AGameManager* gm = Cast<AGameManager>(found[0]))
	{
		gm->ShowDeathScreen();
	}
}

void AGameJam1Character::LockMoveInput(bool a_lock)
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->SetIgnoreMoveInput(a_lock);
	}
}


void AGameJam1Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AGameJam1Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGameJam1Character::ApplyHit(AActor* Source)
{
	const float now = GetWorld()->GetTimeSeconds();
	if (now - LastHitTime < HitCooldown) return;
	LastHitTime = now;

	m_Health = FMath::Clamp(m_Health - 1, 0, m_MaxHealth);

	m_playerUI->UpdateHealthUI(m_Health);

	FVector dir = FVector::ZeroVector;
	if (Source)
	{
		dir = GetActorLocation() - Source->GetActorLocation();
		dir.Z = 0.f;
		if (!dir.Normalize())
		{
			dir = -GetActorForwardVector();
			dir.Z = 0.f; dir.Normalize();
		}
	}
	else
	{
		dir = -GetActorForwardVector();
		dir.Z = 0.f; dir.Normalize();
	}

	const FVector launchVec = (dir * HitBackImpulse) + FVector(0, 0, HitUpImpulse);

	LaunchCharacter(launchVec, true, true);

	if (m_Health <= 0)
	{
		Die();
	}

	GetMesh()->PlayAnimation(m_HitAnim, false);
}

void AGameJam1Character::InstakillPlayer()
{
	const float now = GetWorld()->GetTimeSeconds();
	if (now - LastHitTime < HitCooldown) return;
	LastHitTime = now;

	m_Health = FMath::Clamp(m_Health - 99, 0, m_MaxHealth);

	if (m_Health <= 0)
	{
		Die();
	}

	GetMesh()->PlayAnimation(m_HitAnim, false);

	m_playerUI->UpdateHealthUI(m_Health);
}

void AGameJam1Character::ChangeCookieAmount(int32 a_Amount)
{
	m_PlayerCookieAmount += a_Amount;
	m_playerUI->UpdateCookieUI(m_PlayerCookieAmount);
}


void AGameJam1Character::BeginPlay()
{
	Super::BeginPlay();
	//GetCharacterMovement()->GravityScale = 1.0f;
	if (m_IdleAnim)
	{
		GetMesh()->PlayAnimation(m_IdleAnim, true);
		m_CurrentAnim = m_IdleAnim;
	}

	if (m_playerUIClass)
	m_playerUI = CreateWidget<UPlayerUI>(GetWorld(), m_playerUIClass);
	if (m_playerUI)
	{
		m_playerUI->AddToViewport();
		m_playerUI->UpdateCookieUI(m_PlayerCookieAmount);
	}
}

void AGameJam1Character::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (m_Health <= 0) return;

	if (UAnimSingleNodeInstance* inst = GetMesh()->GetSingleNodeInstance())
	{
		if (m_HitAnim && inst->GetAnimationAsset() == m_HitAnim && inst->IsPlaying()) return;
		else if (inst->GetAnimationAsset() == m_JumpAnim && inst->IsPlaying()) return;
	}

	const float speed = GetVelocity().Size2D();
	const bool  isFalling = GetCharacterMovement()->IsFalling();

	UAnimSequence* desired = nullptr;

	if (isFalling && m_FallAnim)
	{
		desired = m_FallAnim;
	}
	else if (speed > m_WalkSpeedThreshold && m_WalkAnim)
	{
		desired = m_WalkAnim;
	}
	else
	{
		desired = m_IdleAnim;
	}

	if (desired && desired != m_CurrentAnim)
	{
		GetMesh()->PlayAnimation(desired, true);
		m_CurrentAnim = desired;
	}

	constexpr float MaxGlideFallSpeed = 250.f;
	if (auto* Move = GetCharacterMovement())
	{
		FVector V = Move->Velocity;
		if (V.Z < -MaxGlideFallSpeed)
		{
			V.Z = -MaxGlideFallSpeed;
			Move->Velocity = V;
		}
	}

	m_playerUI->UpdateHealthUI(m_Health);
}

void AGameJam1Character::Jump()
{
	if (m_JumpAnim)
	{
		GetMesh()->PlayAnimation(m_JumpAnim, false);
	}

	Super::Jump();
}

float AGameJam1Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageAmount > 0.f)
	{
		ApplyHit(DamageCauser);
	}
	return DamageAmount;
}
