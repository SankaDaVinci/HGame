// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//PlayerCapsule Setup
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.f);

	//Camera setup
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	//Player Init
	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	if (PlayerController)
	{

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}
	
	PlayerSetup();

}



void APlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementValue = Value.Get<FVector2D>();

	const FVector Forward = GetActorForwardVector();
	AddMovementInput(Forward, MovementValue.Y);
	const FVector Right = GetActorRightVector();
	AddMovementInput(Right, MovementValue.X);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void APlayerCharacter::Jumping(const FInputActionValue& Value)
{
	const bool JumpingValue = Value.Get<bool>();
	if (GetController())
	{
		if (bIsFalling)
		{

		}
		else
		{
			Jump();
			GetWorldTimerManager().ClearTimer(StaminaRecharge);
			GetWorldTimerManager().ClearTimer(StaminaRechargeDelay);
			bInitTimerStaminaRechargeDelay = false;
			bIsJumping = true;
			if (bIsSprinting)
			{
				CurrentStamina = CurrentStamina - 15.f;
			}
			
			bIsFalling = true;
		}
		

	}
}

void APlayerCharacter::Sprinting(const FInputActionValue& Value)
{
	
	if (bDisableSprintInput)
	{


	}
	else
	{
		const bool IsSprintingPressed = Value.Get<bool>();
		if (GetController() && IsSprintingPressed && bIsMoving && bEnableStamina)
		{
			SprintStart();
		}
		else
		{
			SprintEnd();
		}
	}
	

}

void APlayerCharacter::PlayerSetup()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	CurrentStamina = MaxStamina;
	UE_LOG(LogTemp, Warning, TEXT("Player Initialized"));
}

//Stamina Gain function adds Stamina to the character
void APlayerCharacter::StaminaGain()
{
	
	bTimerInitiated = true;
	CurrentStamina = CurrentStamina + RechargingStamina;
	if (CurrentStamina >= MaxStamina)
	{
		bDisableSprintInput = false;
		bStaminaDepleted = false;
		bTimerInitiated = false;
		bCanSprint = true;
		CurrentStamina = MaxStamina;
		GetWorldTimerManager().ClearTimer(StaminaRecharge);
	}
	UE_LOG(LogTemp, Warning, TEXT("Stamina Recharging"), RechargingStamina);
}


void APlayerCharacter::StaminaRechargeStartDelay()
{
	if (bInitTimerStaminaRechargeDelay)
	{

	}
	else
	{
		bInitTimerStaminaRechargeDelay = true;
		GetWorldTimerManager().SetTimer(StaminaRechargeDelay, this, &APlayerCharacter::InitTimer, 2.0f, false);

		UE_LOG(LogTemp, Warning, TEXT("Stamina Recharge Delay"));
	}
		
	
}

void APlayerCharacter::InitTimer()
{
	bInitTimerStaminaRechargeDelay = false;
	GetWorldTimerManager().SetTimer(StaminaRecharge, this, &APlayerCharacter::StaminaGain, RechargingIntervalRate, true);

}



void APlayerCharacter::SprintStart()
{
	if (bCanSprint, CurrentStamina > 0 && bStaminaDepleted == false)
	{
		
		
		if (bIsJumping)
		{
			
		}
		else
		{
			GetWorldTimerManager().ClearTimer(StaminaRecharge);
			GetWorldTimerManager().ClearTimer(StaminaRechargeDelay);
			GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
			CurrentStamina = CurrentStamina - StaminaConsumption;
			bIsSprinting = true;
		}
		


	}
	else if (CurrentStamina <= 0)
	{

		bDisableSprintInput = true;
		bStaminaDepleted = true;
		bCanSprint = false;
		CurrentStamina = 0;
		if (bTimerInitiated)
		{

		}
		else
		{
			
			StaminaRechargeStartDelay();
		}

		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		bCanSprint = false;
	}
}

void APlayerCharacter::SprintEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	bIsSprinting = false;
	if (bTimerInitiated)
	{

	}
	else
	{

		StaminaRechargeStartDelay();
		
	}
}

void APlayerCharacter::PlayerVelocity()
{
	float PlayerForwardVelocity = FVector::DotProduct(GetVelocity(), GetActorForwardVector());
	float PlayerSideVelocity = FVector::DotProduct(GetVelocity(), GetActorRightVector());
	float GetPlayerVelocity = PlayerForwardVelocity + PlayerSideVelocity;
	if (GetPlayerVelocity > 0.1)
	{
		bIsMoving = true;
	}
	else
	{
		bIsMoving = false;
	}
}


// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PlayerVelocity();

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Jumping);

		//Sets Sprinting Action In game
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Sprinting);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::SprintEnd);

		
		//EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Triggered, this, &AMainPlayer::ClickingAction);*/
	}

}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	GetWorldTimerManager().ClearTimer(StaminaRecharge);
	StaminaRechargeStartDelay();
	bIsFalling = false;
	if (bIsJumping)
	{
		bIsJumping = false;
		
	}
	if (GEngine)
	{

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("LANDED() Lets do fall damage?"));
	}
}

