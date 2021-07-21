// Fill out your copyright notice in the Description page of Project Settings.


#include "EndGameVolume.h"
#include "PlayerCharacter.h"

#include "Components/BoxComponent.h"

// Sets default values
AEndGameVolume::AEndGameVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	Scene = RootComponent;

	RangeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RangeBox"));
	RangeBox->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AEndGameVolume::BeginPlay()
{
	Super::BeginPlay();
	
	RangeBox->OnComponentBeginOverlap.AddDynamic(this, &AEndGameVolume::OnBeginOverlap);
}

// Called every frame
void AEndGameVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEndGameVolume::OnBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
}