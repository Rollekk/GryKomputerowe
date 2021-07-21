// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GRYKOMPUTEROWE_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UUserWidget> PlayerInterfaceAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	UUserWidget* PlayerInterface;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cursor")
	FRotator CursorLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float playerDeaths;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float enemiesKilled;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float playerDash;

protected:

	virtual void BeginPlay() override;

	FHitResult HitResult;
	TArray<TEnumAsByte<EObjectTypeQuery>> Array;

	bool bIsHitting;

	float ValueToRotate;

public:

	void TurnCharacter(class APlayerCharacter* PlayerChar);

	FORCEINLINE FRotator GetCursorLocation() { return CursorLocation; }
};
