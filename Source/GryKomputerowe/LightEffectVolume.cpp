// Fill out your copyright notice in the Description page of Project Settings.


#include "LightEffectVolume.h"
#include "PlayerCharacter.h"

#include "Components/PointLightComponent.h"
#include "Components/SceneComponent.h"

ALightEffectVolume::ALightEffectVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	Lightning = CreateDefaultSubobject<UPointLightComponent>(TEXT("Lightning"));
	Lightning->SetupAttachment(GetRootComponent());

	HPRegenRate = 2.f;
	StaminaRegenRate = 4.f;
}

void ALightEffectVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsOverlapping && !IsHidden()) ExecuteLightEffect(DeltaTime);
}

void ALightEffectVolume::ExecuteLightEffect(float DeltaTime)
{
	if (Player->PlayerHP > 0.f)
	{
		Player->PlayerHP -= DeltaTime * HPRegenRate;
	}
	if (Player->ActualStaminaCharges > 0.f)
	{
		Player->ActualStaminaCharges -= DeltaTime / StaminaRegenRate;
	}
}