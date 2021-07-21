// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectToHideIn.h"
#include "PlayerCharacter.h"
#include "Sword.h"

#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"

#include "Kismet/GameplayStatics.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AObjectToHideIn::AObjectToHideIn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(GetRootComponent());

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());

	HideKeybind = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HideKeybind"));
	HideKeybind->SetupAttachment(GetRootComponent());

	bIsCollision = false;
}

// Called when the game starts or when spawned
void AObjectToHideIn::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AObjectToHideIn::OnBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AObjectToHideIn::OnEndOverlap);

	HideKeybind->SetHiddenInGame(true);

	OriginalKeybindRotation = HideKeybind->GetRelativeRotation();
}

// Called every frame
void AObjectToHideIn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsCollision)
	{
		CheckForInput();
		UpdateTextRotation();
	}
}

void AObjectToHideIn::UpdateTextRotation()
{
	if (Player)
	{
		FRotator CameraRotation = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetControlRotation();
		HideKeybind->SetWorldRotation(FRotator(OriginalKeybindRotation.Pitch, (CameraRotation.Yaw + OriginalKeybindRotation.Yaw), OriginalKeybindRotation.Roll));
	}
}

void AObjectToHideIn::CheckForInput()
{
	if (Player && Player->bIsInteracting)
	{
		if (!Player->IsHidden())
			HidePlayer();
		else ShowPlayer();
	}
}

void AObjectToHideIn::HidePlayer()
{
	OriginalPlayerLocation = Player->GetActorLocation();

	Player->GetMovementComponent()->StopMovementImmediately();

	Player->SetActorHiddenInGame(true);
	Player->GetSword()->SetActorHiddenInGame(true);
	Player->SetCharacterFreeze(true);
	Player->SetInteracting(false);

	Player->SetActorLocation(GetActorLocation());
}

void AObjectToHideIn::ShowPlayer()
{
	Player->SetActorHiddenInGame(false);
	Player->GetSword()->SetActorHiddenInGame(false);
	Player->SetCharacterFreeze(false);
	Player->SetInteracting(false);

	Player->SetActorLocation(OriginalPlayerLocation);
}

void AObjectToHideIn::OnBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<class APlayerCharacter>(OtherActor))
	{
		bIsCollision = true;
		HideKeybind->SetHiddenInGame(false);

		Player = Cast<class APlayerCharacter>(OtherActor);
	}
}

void AObjectToHideIn::OnEndOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (Cast<class APlayerCharacter>(OtherActor))
	{
		bIsCollision = false;
		HideKeybind->SetHiddenInGame(true);
	}
}