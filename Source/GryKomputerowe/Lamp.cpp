// Fill out your copyright notice in the Description page of Project Settings.


#include "Lamp.h"
#include "LightEffectVolume.h"
#include "Shurikens.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

#include "Engine/PointLight.h"

#include "TimerManager.h"

// Sets default values
ALamp::ALamp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	Scene = RootComponent;

	LampMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LampMesh"));
	LampMesh->SetupAttachment(GetRootComponent());

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisioBox"));
	CollisionBox->SetupAttachment(GetRootComponent());


}

// Called when the game starts or when spawned
void ALamp::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ALamp::OnBeginOverlap);
}

// Called every frame
void ALamp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALamp::TurnOnLight()
{
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	LampLight->SetActorHiddenInGame(false);
}

void ALamp::TurnOffLight()
{
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	LampLight->SetActorHiddenInGame(true);
}

void ALamp::OnBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<class AShurikens>(OtherActor))
	{
		TurnOffLight();
		GetWorldTimerManager().SetTimer(LightOnTimer, this, &ALamp::TurnOnLight, 2.5f);
	}
}



