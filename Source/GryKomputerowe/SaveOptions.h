// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveOptions.generated.h"

/**
 * 
 */
UCLASS()
class GRYKOMPUTEROWE_API USaveOptions : public USaveGame
{
	GENERATED_BODY()

public:

	USaveOptions();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MusicVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UIVolume;
};
