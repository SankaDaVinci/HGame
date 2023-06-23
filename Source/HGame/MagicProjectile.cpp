// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicProjectile.h"
#include "PlayerCharacter.h"


// Sets default values
AMagicProjectile::AMagicProjectile()
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}
	if (!CollisionComponent)
	{
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollisionComponent"));
		CollisionComponent->InitSphereRadius(15.0f);
	}
	RootComponent = CollisionComponent;
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMagicProjectile::OnSphereBeginOverlap);
	
}

void AMagicProjectile::ClearDestroyTimer()
{
	GetWorldTimerManager().ClearTimer(LifeSpan);
}




// Called when the game starts or when spawned
void AMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	CountDownToDestroy();	
	
}

void AMagicProjectile::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, "OnHit() called");
}

void AMagicProjectile::CountDownToDestroy()
{
	GetWorldTimerManager().SetTimer(LifeSpan, this, &AMagicProjectile::DestroyMagic, LifeSpanInSeconds, false);
}

void AMagicProjectile::DestroyMagic()
{
	GetWorldTimerManager().ClearTimer(LifeSpan);
	Destroy();
}

// Called every frame
void AMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

