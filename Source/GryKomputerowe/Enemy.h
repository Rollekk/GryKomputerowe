// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyMovementStatus :uint8
{
	EMS_Idle UMETA(DeplayName = "Idle"),
	EMS_MoveBack UMETA(DeplayName = "MoveBack"),
	EMS_MoveToTarget UMETA(DeplayName = "MoveToTarget"),
	EMS_Attacking UMETA(DeplayName = "Attacking"),

	EMS_MAX UMETA(DeplayName = "DefaultMAX")
};

UCLASS()
class GRYKOMPUTEROWE_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Movement")
	EEnemyMovementStatus EnemyMovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	class AMainPlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UStaticMeshComponent* AggroMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class USphereComponent* CombatSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UWidgetComponent* HealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController* AIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class APlayerCharacter* TargetToFollow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TArray<class ATargetPoint*> PathArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword")
	class ASword* Sword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword")
	TSubclassOf<class ASword> EnemySword;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* CombatMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* SwordMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float EnemyHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float EnemyDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bCombatIsOverlapping;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timer")
	FTimerHandle FollowingTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timer")
	FTimerHandle DeathTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	float StopFollowingTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LinearColor")
	FLinearColor AggroColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LinearColor")
	FLinearColor DefaultColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Index")
	int ArrayIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float MaxMoveBackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float MaxWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	FRotator OriginalEnemyRotation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bDeath;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FRotator OriginalKeybindRotation;

	void EnumCheck();
	void UpdateBarRotation();

	void SetupSword();

	bool bIsSwordActive;
	void EquipSword();
	void UnequipSword();

	void SetAggroMaterial(FString name);

	void CancelCombatMontage();
	void StopFollowingTarget();

	void Dead();
	bool bIsDead;
	bool bCanDestroy;
	void PlayDeadAnimation(float animPlayRate);

	class ATargetPoint* GetAINextLocation();

	UFUNCTION(BlueprintCallable)
	void MoveToTarget(class APlayerCharacter* Target);

	UFUNCTION()
	void AggroOnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void AggroOnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void CombatOnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void CombatOnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FORCEINLINE void SetEnemyMovementStatus(EEnemyMovementStatus Status) { EnemyMovementStatus = Status; }
};
