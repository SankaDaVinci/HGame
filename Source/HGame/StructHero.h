// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StructHero.generated.h"

USTRUCT(BlueprintType)
struct HGAME_API FStructHero
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	float fDefense;

	UPROPERTY(BlueprintReadWrite)
	float fAttackSpeed;

	UPROPERTY(BlueprintReadWrite)
	float fMoveSpeed;

	UPROPERTY(BlueprintReadWrite)
		float fMaxHealth;

	UPROPERTY(BlueprintReadWrite)
		float fHealthRegenRate;

	UPROPERTY(BlueprintReadWrite)
		float fMaxMana;

	UPROPERTY(BlueprintReadWrite)
		float fManaRegenRate;

	UPROPERTY(BlueprintReadWrite)
		float fMaxStamina;

	UPROPERTY(BlueprintReadWrite)
		float fStaminaRegenRate;

	UPROPERTY(BlueprintReadWrite)
		float fStaminaConsumption;





};
