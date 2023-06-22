
// Fill out your copyright notice in the Description page of Project Settings.

#include "Player_PickUp_Component.h"

void UPlayer_PickUp_Component::UTP_PickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UPlayer_PickUp_Component::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UPlayer_PickUp_Component::OnSphereBeginOverlap);
}

void UPlayer_PickUp_Component::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* PChar = Cast<APlayerCharacter>(OtherActor);
	

	if (PChar != nullptr)
	{
		OnPickUp.Broadcast(PChar);

		//Removing the Register event
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
