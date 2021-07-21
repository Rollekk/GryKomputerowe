// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectVolume.h"
#include "LightEffectVolume.generated.h"

/**
 * 
 */
UCLASS()
class GRYKOMPUTEROWE_API ALightEffectVolume : public AEffectVolume
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALightEffectVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Scene")
	class USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Light")
	class UPointLightComponent* Lightning;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ExecuteLightEffect(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HP")
	float HPRegenRate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina")
	float StaminaRegenRate;
};
