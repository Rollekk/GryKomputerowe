// Fill out your copyright notice in the Description page of Project Settings.


#include "Shurikens.h"
#include "PlayerCharacter.h"
#include "Enemy.h"

#include "Components/SceneComponent.h"

#include "Kismet/GameplayStatics.h"

#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include "Sound/SoundCue.h"

// Sets default values
AShurikens::AShurikens()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	ShurikenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShurikenMesh"));
	ShurikenMesh->SetupAttachment(GetRootComponent());

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	ShurikenDamage = 5.f;
	HitSoundVolume = 0.5f;
}

// Called when the game starts or when spawned
void AShurikens::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<class APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	ShurikenMesh->OnComponentBeginOverlap.AddDynamic(this, &AShurikens::OnBeginOverlap);
}

// Called every frame
void AShurikens::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalRotation(FRotator(0.f, 20.f, 0.f));
}

void AShurikens::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		UGameplayStatics::PlaySound2D(this, HitSound, HitSoundVolume);
		if (Cast<AEnemy>(OtherActor))
		{
			AEnemy* Enemy = Cast<AEnemy>(OtherActor);
			Enemy->EnemyHealth -= ShurikenDamage;
			if (!Cast<class APlayerCharacter>(Enemy->TargetToFollow))
			{
				Enemy->TargetToFollow = Player;
				Enemy->MoveToTarget(Player);
			}
		}
		Destroy();
	}
}
