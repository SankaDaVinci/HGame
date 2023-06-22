// Fill out your copyright notice in the Description page of Project Settings.


#include "FP_Weapon.h"
#include "GameFramework/PlayerController.h"
#include "MagicProjectile.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

UFP_Weapon::UFP_Weapon()
{
	WandTipOffset = FVector(0.f, 0.f, 0.f);
}

void UFP_Weapon::AttachWandToPlayer(APlayerCharacter* TargetCharacter)
{
	
	Character = TargetCharacter;
	
	if (Character == nullptr)
	{
		
		return;
	}

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));



	Character->SetHasWeapon(true);

	

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			InputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UFP_Weapon::Fire);
		}
	}

}

void UFP_Weapon::Fire()
{
	
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());

			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();

			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(WandTipOffset);

			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			World->SpawnActor<AMagicProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

		}
	}

	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());

	}

	if (FireAnimation != nullptr)
	{
		UAnimInstance* AnimationInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimationInstance != nullptr)
		{
			AnimationInstance->Montage_Play(FireAnimation, 1.f);
		}

	}



}

void UFP_Weapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	/*if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}*/
}
