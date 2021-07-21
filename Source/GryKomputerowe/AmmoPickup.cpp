// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickup.h"
#include "PlayerCharacter.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/SceneComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AAmmoPickup::AAmmoPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(GetRootComponent());

	PickupKeybind = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PickupKeybind"));
	PickupKeybind->SetupAttachment(GetRootComponent());

	bIsCollision = false;
	SizeOfAmmo = 1.f;
}

// Called when the game starts or when spawned
void AAmmoPickup::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AAmmoPickup::OnBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AAmmoPickup::OnEndOverlap);

	PickupKeybind->SetHiddenInGame(true);
	OriginalKeybindRotation = PickupKeybind->GetRelativeRotation();
}

// Called every frame
void AAmmoPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsCollision)
	{
		CheckForInput();
		UpdateTextRotation();
	}
}

void AAmmoPickup::UpdateTextRotation()
{
	if (Player)
	{
		FRotator CameraRotation = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetControlRotation();
		PickupKeybind->SetWorldRotation(FRotator(OriginalKeybindRotation.Pitch,(CameraRotation.Yaw + OriginalKeybindRotation.Yaw),OriginalKeybindRotation.Roll));
	}
}

void AAmmoPickup::CheckForInput()
{
	if (Player)
	{
		if (Player->bIsInteracting && Player->PlayerShurikens <= 2.f)
		{
			Player->PlayerShurikens += SizeOfAmmo;
			Player->SetInteracting(false);
			Destroy();
		}
	}
}

void AAmmoPickup::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<class APlayerCharacter>(OtherActor))
	{
		bIsCollision = true;
		PickupKeybind->SetHiddenInGame(false);

		Player = Cast<class APlayerCharacter>(OtherActor);
	}
}

void AAmmoPickup::OnEndOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (Cast<class APlayerCharacter>(OtherActor))
	{
		bIsCollision = false;
		PickupKeybind->SetHiddenInGame(true);
	}
}

