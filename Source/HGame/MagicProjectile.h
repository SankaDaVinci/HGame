// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "MagicProjectile.generated.h"


UCLASS()
class HGAME_API AMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagicProjectile();
	
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UFUNCTION(BlueprintCallable, Category = "Timer Handle")
		void ClearDestroyTimer();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Setup")
	float LifeSpanInSeconds = 2;

	FTimerHandle LifeSpan;

	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void CountDownToDestroy();

	void DestroyMagic();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
