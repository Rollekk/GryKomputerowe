// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectToHideIn.generated.h"

UCLASS()
class GRYKOMPUTEROWE_API AObjectToHideIn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectToHideIn();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	class APlayerCharacter* Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Scene")
	class USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RenderText")
	class UTextRenderComponent* HideKeybind;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FRotator OriginalKeybindRotation;
	FVector OriginalPlayerLocation;

	void UpdateTextRotation();

	bool bIsCollision;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CheckForInput();
	
	void HidePlayer();
	void ShowPlayer();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
