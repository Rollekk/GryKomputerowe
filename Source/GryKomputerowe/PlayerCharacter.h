// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class GRYKOMPUTEROWE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	class AMainPlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class ASword* Sword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<class ASword> SpawnThisSword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class AShurikens* Shurikens;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<class AShurikens> SpawnThisShurikens;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<class APlayerCharacter> SpawnThisPlayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* CombatMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* SwordMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	FTimerHandle Timer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	FTimerHandle DeadTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Location")
	FVector RespawnLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rotation")
	FRotator RespawnRotation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ZoomIn();
	void ZoomOut();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float ZoomSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MaxZoomOut;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MaxZoomIn;

	void MoveSideways(float Value);
	void MoveForward(float Value);

	void Turn(float Value);

	void StartCrouching();
	bool bIsCrouching;

	void Jump();
	void StopJumping();

	void Dead();
	void PlayDeadAnimation(float animPlayRate);
	void StopAnim();
	UFUNCTION(BlueprintCallable)
	void Respawn();

	bool bIsDead;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsInCombat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bCharacterFreeze;

	void Dash();
	void StopDash();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float DashDistance;
	bool bIsDashing;

	void SetupSword();
	bool bIsSwordActive;
	void ChangeSwordPosition();
	void Draw();
	void Sheath();

	void RangeAttack();
	UFUNCTION(BlueprintCallable)
	void SpawnShuriken();

	void UseSword();

	void Interact();
	void StopInteracting();
	bool bIsInteracting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Light")
	TArray<class APointLight*> PointLightsArray;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Light")
	TArray<float> DistanceArray;
	int32 DistanceIndex;

	void SetupLights();
	float GetLightAmount();

	void BalanceHealth(float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowPauseMenu();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HP")
	float PlayerHP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammunition")
	float PlayerShurikens;

	void StaminaChargeLogic(float DrainRate);
	float MaxStaminaCharges;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina")
	float ActualStaminaCharges;
	int MinStaminaCharges;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina")
	float StaminaDrainRate;

	FORCEINLINE void SetShurikens(class AShurikens* ToThis) { Shurikens = ToThis; }
	FORCEINLINE void SetCharacterFreeze(bool ToThis) { bCharacterFreeze = ToThis;  }
	FORCEINLINE void SetInteracting(bool ToThis) { bIsInteracting = ToThis; }
	FORCEINLINE class ASword* GetSword() { return Sword;  }

};
