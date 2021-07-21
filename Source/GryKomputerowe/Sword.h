// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sword.generated.h"

UCLASS()
class GRYKOMPUTEROWE_API ASword : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASword();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	class APlayerCharacter* Player;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sword")
	class UStaticMeshComponent* SwordMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	class UBoxComponent* SwordCollision;;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timer")
	FTimerHandle CounterResetTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timer")
	FTimerHandle AnimationTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timer")
	float TimerAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	class ACharacter* CharacterAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	UAnimMontage* CombatMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	UAnimMontage* SwordMontage;

	float AttackCounter;
	bool bIsAttacking;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Draw();
	void Sheath();

	void Attack(float animPlayRate);
	void AddToCounter();
	void ResetCounter();

	void PlaySwordAnimation(FName SectionName);
	void PlayAttackAnimation(float animPlayRate);

	void StopAttackingAfterAnim();

	UFUNCTION(BlueprintCallable)
	void ActivateCollision();
	UFUNCTION(BlueprintCallable)
	void EndCollision();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FORCEINLINE void SetCharacterAttacking(ACharacter* ToThis) { CharacterAttacking = ToThis;  }
	FORCEINLINE void SetCombatMontage(UAnimMontage* ToThis) { CombatMontage = ToThis; }
	FORCEINLINE void SetSwordMontage(UAnimMontage* ToThis) { SwordMontage = ToThis; }
};
