// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "PlayerCharacter.h"
#include "Engine.h"

#include "Blueprint/UserWidget.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerInterfaceAsset)
	{
		PlayerInterface = CreateWidget<UUserWidget>(this, PlayerInterfaceAsset);
		if (PlayerInterface && GetWorld()->GetName() != "MainMenu")
		{
			PlayerInterface->AddToViewport();
			PlayerInterface->SetVisibility(ESlateVisibility::Visible);
		}
	}

	bShowMouseCursor = true;
	bIsHitting = false;

	ValueToRotate = 0.f;

	playerDeaths = 0.f;
	enemiesKilled = 0.f;
	playerDash = 0.f;
}

void AMainPlayerController::TurnCharacter(class APlayerCharacter* PlayerChar)
{
	if (PlayerChar && !IsPaused())
	{

		bIsHitting = GetHitResultUnderCursorForObjects(Array, true, HitResult);

		if (bIsHitting)
		{
			CursorLocation = UKismetMathLibrary::FindLookAtRotation(PlayerChar->GetActorLocation(), HitResult.Location);
			FRotator PlayerLocation = PlayerChar->GetActorRotation();

			ValueToRotate = CursorLocation.Yaw - PlayerLocation.Yaw;
			PlayerChar->AddActorWorldRotation(FRotator(0.f, ValueToRotate, 0.f));
		}

	}
}