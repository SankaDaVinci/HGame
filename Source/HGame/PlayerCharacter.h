// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UCameraComponent;

UCLASS()
class HGAME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;
public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		bool bHasWeapon;

	UFUNCTION(BlueprintCallable, Category = "Player Action")
		void SetHasWeapon(bool bNewHasWeapon);

	UFUNCTION(BlueprintCallable, Category = "Player Action")
		bool GetHasWeapon();

	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FirstPersonCameraComponent;

	//Allows to set the Mapping Context on class default
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* SprintAction;

	//Time Handlers	
	FTimerHandle StaminaRecharge;

	FTimerHandle StaminaRechargeDelay;

	//Creating the movement FUNCTION that its functionality is declared at the .cpp 
	void Move(const FInputActionValue& Value);

	//Creating the Look FUNCTION that its functionality is declared at the .cpp 
	void Look(const FInputActionValue& Value);

	void Jumping(const FInputActionValue& Value);

	void Sprinting(const FInputActionValue& Value);

	UFUNCTION(Category = "PlayerInit")
	void PlayerSetup();

	UFUNCTION(Category = "PlayerSettings")
	void StaminaGain();

	UFUNCTION(Category = "Timer Handle")
	void InitTimer();

	UFUNCTION(Category = "Timer Handle")
	void StaminaRechargeStartDelay();

	UFUNCTION(Category = "PlayerMovement")
		void SprintStart();

	UFUNCTION(Category = "PlayerMovement")
		void SprintEnd();

	UFUNCTION()
	void PlayerVelocity();

	



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Settings|Settings|Stamina")
	bool bEnableStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bEnableStamina"), Category = "Actor Settings|Settings|Stamina");
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0, ClampMax = 2000), Category = "Player Status");
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0, ClampMax = 2000, EditCondition = "bEnableStamina"), Category = "Actor Settings|Settings|Stamina");
	float StaminaConsumption;

	UPROPERTY(BlueprintReadWrite, Category = "Actor Settings|Settings|Stamina");
	float SprintSpeed = 600;

	UPROPERTY(BlueprintReadWrite, Category = "Player Action");
	bool bCanSprint = true;

	UPROPERTY(BlueprintReadOnly, meta = (ClampMin = 0, ClampMax = 100), Category = "Actor Settings|Settings|Stamina")
	float CurrentStamina = 0.f;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Actor Settings|Settings|Stamina")
	float RechargingStamina = 2.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Actor Settings|Settings|Stamina")
	float RechargingIntervalRate;

	UPROPERTY(BlueprintReadOnly, Category = "Player Status")
	bool bStaminaDepleted;

	UPROPERTY(BlueprintReadOnly, Category = "Player Status")
	bool bDisableSprintInput;

	UPROPERTY(BlueprintReadOnly, Category = "Timer Handle")
	bool bTimerInitiated;

	UPROPERTY(BlueprintReadWrite, Category = "Actor Settings|Player Status")
	bool bIsMoving;

	UPROPERTY(BlueprintReadOnly, Category = "Actor Protected|Stamina")
	bool bInitTimerStaminaRechargeDelay;

	UPROPERTY(BlueprintReadOnly, Category = "Player Action")
	bool bIsJumping;

	UPROPERTY(BlueprintReadOnly, Category = "Player Action")
	bool bIsFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Player Action")
	bool bIsSprinting;

	

	

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Landed(const FHitResult& Hit) override;

};
