// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowEffectVolume.h"
#include "PlayerCharacter.h"

AShadowEffectVolume::AShadowEffectVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HPRegenRate = 2.f;
	StaminaRegenRate = 4.f;
}

void AShadowEffectVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsOverlapping) ExecuteShadowEffect(DeltaTime);
}

void AShadowEffectVolume::ExecuteShadowEffect(float DeltaTime)
{
	if (Player->PlayerHP < 100.f)
	{
		Player->PlayerHP += DeltaTime * HPRegenRate;
	}
	if (Player->ActualStaminaCharges < Player->MaxStaminaCharges)
	{
		Player->ActualStaminaCharges += DeltaTime / StaminaRegenRate;
	}
}