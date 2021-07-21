// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shurikens.generated.h"

UCLASS()
class GRYKOMPUTEROWE_API AShurikens : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShurikens();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	class APlayerCharacter* Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	class USceneComponent* Scene;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shuriken")
	class UStaticMeshComponent* ShurikenMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shuriken")
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* CombatMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundCue* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float ShurikenDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float HitSoundVolume;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
