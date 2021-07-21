// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectVolume.h"
#include "ShadowEffectVolume.generated.h"

/**
 * 
 */
UCLASS()
class GRYKOMPUTEROWE_API AShadowEffectVolume : public AEffectVolume
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShadowEffectVolume();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ExecuteShadowEffect(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HP")
	float HPRegenRate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina")
	float StaminaRegenRate;
};
