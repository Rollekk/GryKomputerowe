// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "PlayerCharacter.h"
#include "MainPlayerController.h"
#include "Sword.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "AIController.h"
#include "TimerManager.h"

#include "Animation/AnimInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/TargetPoint.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AggroMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AggroMesh"));
	AggroMesh->SetupAttachment(GetRootComponent());
	AggroMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(GetRootComponent());

	bCombatIsOverlapping = false;
	bIsSwordActive = false;
	bIsDead = false;
	bCanDestroy = true;
	bDeath = false;

	EnemyHealth = 100.f;
	EnemyDamage = 25.f;

	ArrayIndex = 0;
	StopFollowingTimer = 1.f;

	MaxMoveBackSpeed = 130.f;
	MaxWalkSpeed = 300.f;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<class AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	AIController = Cast<AAIController>(GetController());

	SetupSword();

	AggroMesh->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AggroOnBeginOverlap);
	AggroMesh->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AggroOnEndOverlap);

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatOnBeginOverlap);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatOnEndOverlap);
	
	OriginalKeybindRotation = HealthBar->GetRelativeRotation();
	if(PathArray.Num() == 1.f) OriginalEnemyRotation = GetActorRotation();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsDead)
	{
		if (bIsSwordActive && bCombatIsOverlapping)	Sword->Attack(0.75f);
		if (TargetToFollow && TargetToFollow->IsHidden()) StopFollowingTarget();

		EnumCheck();
		UpdateBarRotation();
	}
	if (EnemyHealth <= 0.f) Dead();
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::EnumCheck()
{
	if (EnemyMovementStatus == EEnemyMovementStatus::EMS_MoveToTarget)
	{
		CancelCombatMontage();
	}
	if (EnemyMovementStatus != EEnemyMovementStatus::EMS_Attacking)
	{
		if (AIController->GetMoveStatus() == EPathFollowingStatus::Idle)
		{
			if(PathArray.Num() != 1.f)
			{
				SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
				if (AIController) AIController->MoveToActor(GetAINextLocation());
			}
			else if(PathArray.Num() == 1.f && !TargetToFollow)
			{
				SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
				SetActorRotation(OriginalEnemyRotation);
			}
		}
	}
	if (EnemyMovementStatus == EEnemyMovementStatus::EMS_MoveBack)
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxMoveBackSpeed;
	}
	else GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}

void AEnemy::UpdateBarRotation()
{
		FRotator CameraRotation = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetControlRotation();
		HealthBar->SetWorldRotation(FRotator(OriginalKeybindRotation.Pitch, (CameraRotation.Yaw + OriginalKeybindRotation.Yaw), OriginalKeybindRotation.Roll));
}

void AEnemy::SetupSword()
{
	Sword = GetWorld()->SpawnActor<ASword>(EnemySword);
	if (Sword)
	{
		Sword->SetCharacterAttacking(this);
		Sword->SetCombatMontage(CombatMontage);
		Sword->SetSwordMontage(SwordMontage);
		Sword->Sheath();
	}
}

void AEnemy::EquipSword()
{
	if (Sword)
	{
		bIsSwordActive = true;
		Sword->PlaySwordAnimation("DrawSword");
	}
}

void AEnemy::UnequipSword()
{
	if (Sword)
	{
		bIsSwordActive = false;
		Sword->PlaySwordAnimation("SheathSword");
	}
}

void AEnemy::SetAggroMaterial(FString name)
{
	UMaterialInterface* AggroMaterial = AggroMesh->GetMaterial(0);
	UMaterialInstanceDynamic* AggroMaterialDynamic;
	AggroMaterialDynamic = AggroMesh->CreateDynamicMaterialInstance(0, AggroMaterial);

	if(name == "Red")
	{
		AggroMaterialDynamic->SetVectorParameterValue(TEXT("Color"), AggroColor);
	}
	else if(name == "Default")
	{
		AggroMaterialDynamic->SetVectorParameterValue(TEXT("Color"), DefaultColor);
	}
}

void AEnemy::CancelCombatMontage()
{
	UAnimInstance* AnimInstance = this->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Stop(0.f, CombatMontage);
}

void AEnemy::StopFollowingTarget()
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveBack);
	UnequipSword();
	if (TargetToFollow)
	{
		TargetToFollow->bIsInCombat = false;
		TargetToFollow = nullptr;
	}
	SetAggroMaterial("Default");
	if (AIController) AIController->MoveToActor(PathArray[ArrayIndex]);
}

void AEnemy::Dead()
{
		bIsDead = true;
		if (TargetToFollow)
		{
			TargetToFollow->bIsInCombat = false;
			TargetToFollow = nullptr;
		}
		if (AIController) AIController->StopMovement();
		AggroMesh->DestroyComponent();

		if (bCanDestroy)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			
			PlayDeadAnimation(1.25f);
			SetLifeSpan(2.4f);
			Sword->SetLifeSpan(2.4f);
			bCanDestroy = false;
		}

		if (!bDeath)
		{
			bDeath = true;
			if(PlayerController) PlayerController->enemiesKilled++;
		}
}

void AEnemy::PlayDeadAnimation(float animPlayRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, animPlayRate);
		AnimInstance->Montage_JumpToSection("Death", CombatMontage);
	}
}

class ATargetPoint* AEnemy::GetAINextLocation()
{
	if (PathArray[ArrayIndex] == PathArray[PathArray.Num()-1])
	{
		ArrayIndex = 0;
	}
	else
	{
		ArrayIndex++;
	}
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveBack);
	if(PathArray.Num() == 1.f) SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveBack);
	return PathArray[ArrayIndex];
}

void AEnemy::AggroOnBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && !bIsDead)
	{
		if (Cast<class APlayerCharacter>(OtherActor))
		{
			SetAggroMaterial("Red");
			TargetToFollow = Cast<class APlayerCharacter>(OtherActor);
			TargetToFollow->bIsInCombat = true;
			GetWorldTimerManager().PauseTimer(FollowingTimer);
			EquipSword();
			MoveToTarget(Cast<class APlayerCharacter>(OtherActor));
		}
	}
}

void AEnemy::AggroOnEndOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor && !bIsDead)
	{
		if (Cast<class APlayerCharacter>(OtherActor)) GetWorldTimerManager().SetTimer(FollowingTimer,this,&AEnemy::StopFollowingTarget, StopFollowingTimer);
	}
}

void AEnemy::CombatOnBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && !bIsDead)
	{
		if (Cast<class APlayerCharacter>(OtherActor))
		{
			bCombatIsOverlapping = true;
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
		}
	}
}

void AEnemy::CombatOnEndOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && !bIsDead)
	{
		if (Cast<class APlayerCharacter>(OtherActor))
		{
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);
			bCombatIsOverlapping = false;
			if (EnemyMovementStatus != EEnemyMovementStatus::EMS_Attacking)
			{
				MoveToTarget(Cast<class APlayerCharacter>(OtherActor));
			}
		}
	}
}

void AEnemy::MoveToTarget(APlayerCharacter* Target)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);

	if (AIController)
	{
			FAIMoveRequest MoveRequest;
			MoveRequest.SetGoalActor(Target);
			MoveRequest.SetAcceptanceRadius(50.f);

			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), Target->GetActorLocation()));

			FNavPathSharedPtr NavPath;
			AIController->MoveTo(MoveRequest, &NavPath);
	}
}