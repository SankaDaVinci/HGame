// Copyright Daniel Alves Souza. All copyrights reserved


#include "MainPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"


// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bool bHasWeapon = false;

	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	FPCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FPCameraComp->SetupAttachment(GetCapsuleComponent());

	if (!CastingArrow)
	{
		CastingArrow = CreateDefaultSubobject<USceneComponent>(TEXT("Casting Arrow"));
		CastingArrow->SetupAttachment(CastingArrow);
	}

}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	
	// Ads the Mapping Context via Blueprint's Class Default
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}

	//Initializes Player
	PlayerSetUp();

	
}

//Move Function
void AMainPlayer::Move(const FInputActionValue& Value)
{
	const FVector2D MovementValue = Value.Get<FVector2D>();
	
	const FVector Forward = GetActorForwardVector();
	AddMovementInput(Forward, MovementValue.Y);
	const FVector Right = GetActorRightVector();
	AddMovementInput(Right, MovementValue.X);
}

//Look Around Function
void AMainPlayer::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void AMainPlayer::Jumping(const FInputActionValue& Value)
{
	const bool JumpingValue = Value.Get<bool>();
	if (GetController())
	{
		Jump();
	}
}

void AMainPlayer::Sprinting(const FInputActionValue& Value)
{
	const bool IsSprintingPressed = Value.Get<bool>();
	if (GetController() && IsSprintingPressed && bIsMoving &&bEnableStamina)
	{
		SprintStart();
	}
	else
	{
		SprintEnd();
	}
}

void AMainPlayer::ClickingAction(const FInputActionValue& Value)
{

	const bool IsLeftClicking = Value.Get<bool>();
	if (GetController() && IsLeftClicking)
		{

		}
		UE_LOG(LogTemp, Warning, TEXT("Calling Click"));
			

}





// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	PlayerVelocity();
	

}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainPlayer::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainPlayer::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMainPlayer::Jumping);

		//Sets Sprinting Action In game
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AMainPlayer::Sprinting);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMainPlayer::SprintEnd);


		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Triggered, this, &AMainPlayer::ClickingAction);
	}
	

}

void AMainPlayer::PlayerSetUp()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	CurrentStamina = MaxStamina;
	UE_LOG(LogTemp, Warning, TEXT("Player Initialized"));
}

void AMainPlayer::StaminaGain()
{
	bTimerInitiated = true;
	CurrentStamina = CurrentStamina + RechargingStamina;
	if (CurrentStamina >= MaxStamina)
	{
		bTimerInitiated = false;
		bCanSprint = true;
		CurrentStamina = MaxStamina;
		GetWorldTimerManager().ClearTimer(StaminaRecharge);
	}
	UE_LOG(LogTemp, Warning, TEXT("Stamina Recharging"), RechargingStamina);
}

void AMainPlayer::InitTimer()
{	
	GetWorldTimerManager().SetTimer(StaminaRecharge, this, &AMainPlayer::StaminaGain, 0.8f, true);
}

void AMainPlayer::StaminaRechargeStartDelay()
{
	GetWorldTimerManager().SetTimer(StaminaRechargeDelay, this, &AMainPlayer::InitTimer, 2.0f, false);
}

/*Checks if player can sprint and does have stamina to sprint*/
void AMainPlayer::SprintStart()
{
	
	
	


	if (bCanSprint, CurrentStamina > 0)
	{
		GetWorldTimerManager().ClearTimer(StaminaRecharge);
		GetWorldTimerManager().ClearTimer(StaminaRechargeDelay);
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		CurrentStamina = CurrentStamina - StaminaConsumption;


	}
	else if (CurrentStamina <= 0)
	{
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
/*Sprint End Function, Sets the player back to the normal walking speed and begins recharging the Stamina*/
void AMainPlayer::SprintEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	if (bTimerInitiated)
	{

	}
	else
	{
		StaminaRechargeStartDelay();
	}

	
}

void AMainPlayer::PlayerVelocity()
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






