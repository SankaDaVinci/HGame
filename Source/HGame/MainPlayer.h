// Copyright Daniel Alves Souza. All copyrights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "InputActionValue.h"
#include "MainPlayer.generated.h"

class UInputMappingContext;
class UInputAction;
class ILayoutDetails;
class UCameraComponent;

UCLASS()
class HGAME_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();

	// Projectile class to spawn
	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<AActor> ProjectileClass;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Player Component")
	USceneComponent* CastingArrow;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* FPCameraComp;





	//Allows to set the Mapping Context on class default
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* PlayerMappingContext;
	
	//Allows to set the InputAction on blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LeftClickAction;

	

	//Creating the movement FUNCTION that its functionality is declared at the .cpp 
	void Move(const FInputActionValue& Value);

	//Creating the Look FUNCTION that its functionality is declared at the .cpp 
	void Look(const FInputActionValue& Value);

	void Jumping(const FInputActionValue& Value);

	void Sprinting(const FInputActionValue& Value);

	void ClickingAction(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Settings|Settings|Stamina")
	bool bEnableStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bEnableStamina"), Category = "Actor Settings|Settings|Stamina");
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin= 0, ClampMax= 2000), Category = "Player Status");
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0, ClampMax = 2000, EditCondition = "bEnableStamina"), Category = "Actor Settings|Settings|Stamina");
	float StaminaConsumption;

	UPROPERTY(BlueprintReadWrite, Category = "Player Status");
	float SprintSpeed = 600;

	UPROPERTY(BlueprintReadWrite, Category = "Player Action");
	bool bCanSprint = true;

	UPROPERTY(BlueprintReadOnly, meta=(ClampMin = 0, ClampMax = 100), Category = "Actor Settings|Settings|Stamina")
	float CurrentStamina = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Player Status")
	float RechargingStamina = 2.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Timer Handle")
	bool bTimerInitiated;

	UPROPERTY(BlueprintReadWrite, Category = "Actor Settings|Player Status")
	bool bIsMoving;

	UPROPERTY(BlueprintReadWrite)
	FTransform Location;

	UPROPERTY(BlueprintReadWrite)
	FRotator Rotation;

	//Time Handlers	
	FTimerHandle StaminaRecharge;

	FTimerHandle StaminaRechargeDelay;

	//Functions
	UFUNCTION(Category = "PlayerInit")
	void PlayerSetUp();

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









public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	
	
	
	

};

