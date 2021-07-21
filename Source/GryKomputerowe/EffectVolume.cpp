// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectVolume.h"
#include "PlayerCharacter.h"

#include "Components/BoxComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AEffectVolume::AEffectVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RangeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RangeBox"));
	RangeBox->SetupAttachment(GetRootComponent());

	bIsOverlapping = false;
}

// Called when the game starts or when spawned
void AEffectVolume::BeginPlay()
{
	Super::BeginPlay();

	RangeBox->OnComponentBeginOverlap.AddDynamic(this, &AEffectVolume::OnBeginOverlap);
	RangeBox->OnComponentEndOverlap.AddDynamic(this, &AEffectVolume::OnEndOverlap);
}

void AEffectVolume::OnBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<class APlayerCharacter>(OtherActor))
	{
		bIsOverlapping = true;

		Player = Cast<class APlayerCharacter>(OtherActor);
	}
}

void AEffectVolume::OnEndOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (Cast<class APlayerCharacter>(OtherActor))
	{
		bIsOverlapping = false;
	}
}

