// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EndGameVolume.generated.h"

UCLASS()
class GRYKOMPUTEROWE_API AEndGameVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEndGameVolume();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scene")
	class USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class UBoxComponent* RangeBox;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
