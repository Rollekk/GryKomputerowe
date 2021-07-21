// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "MainPlayerController.h"
#include "Sword.h"
#include "Shurikens.h"

#include "Camera/CameraComponent.h"

#include "Components/InputComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/SkeletalMeshComponent.h"

#include "Components/PointLightComponent.h"
#include "Engine/PointLight.h"
#include "EngineUtils.h"

#include "TimerManager.h"

#include "Animation/AnimInstance.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ZoomSpeed = 50.f;
	MaxZoomOut = 700.f;
	MaxZoomIn = 500.f;
	DashDistance = 6000.f;

	MaxStaminaCharges = 3.f;
	MinStaminaCharges = 1;
	ActualStaminaCharges = 3.f;
	StaminaDrainRate = 0.3f;

	PlayerHP = 100.f;
	PlayerShurikens = 3.f;

	bIsCrouching = false;
	bIsDashing = false;
	bCharacterFreeze = false;
	bIsDead = false;
	bIsInCombat = false;
	bIsSwordActive = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = MaxZoomIn;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	SetupSword();

	RespawnLocation = GetActorLocation();
	RespawnRotation = GetActorRotation();

	SetupLights();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StaminaChargeLogic(StaminaDrainRate * DeltaTime);
	
	if (PlayerHP <= 0.f) Dead();
	else BalanceHealth(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJumping);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::StartCrouching);

	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &APlayerCharacter::ZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &APlayerCharacter::ZoomOut);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &APlayerCharacter::Dash);

	PlayerInputComponent->BindAction("SwordAction", IE_Pressed, this, &APlayerCharacter::ChangeSwordPosition);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::UseSword);

	PlayerInputComponent->BindAction("Throw", IE_Pressed, this, &APlayerCharacter::RangeAttack);

	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &APlayerCharacter::Interact);
	PlayerInputComponent->BindAction("Interaction", IE_Released, this, &APlayerCharacter::StopInteracting);

	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &APlayerCharacter::ShowPauseMenu);

	PlayerInputComponent->BindAxis("MoveSideways", this, &APlayerCharacter::MoveSideways);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);

	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);
}

void APlayerCharacter::ZoomIn()
{
	if (SpringArm)
	{
		float CurrentTargetArmLength = SpringArm->TargetArmLength;

		if (CurrentTargetArmLength > MaxZoomIn)
		{
			float NewTargetArmLength = CurrentTargetArmLength - ZoomSpeed;

			SpringArm->TargetArmLength = NewTargetArmLength;
		}
	}
}

void APlayerCharacter::ZoomOut()
{
	if (SpringArm)
	{
		float CurrentTargetArmLength = SpringArm->TargetArmLength;

		if (CurrentTargetArmLength < MaxZoomOut)
		{
			float NewTargetArmLength = CurrentTargetArmLength + ZoomSpeed;

			SpringArm->TargetArmLength = NewTargetArmLength;
		}
	}
}

void APlayerCharacter::StartCrouching()
{
	if (!GetCharacterMovement()->IsFalling() && !bCharacterFreeze)
	{
		if (bIsCrouching)
		{
			Super::UnCrouch();
			bIsCrouching = false;
		}
		else
		{
			Super::Crouch();
			bIsCrouching = true;
		}
	}
}

void APlayerCharacter::Jump()
{
	if (!bCharacterFreeze)
	{
		ACharacter::Jump();
	}
}

void APlayerCharacter::StopJumping()
{
	if (!bCharacterFreeze) ACharacter::StopJumping();
}

void APlayerCharacter::Dead()
{
	if (!bIsDead)
	{
		bIsDead = true;
		bCharacterFreeze = true;

		PlayDeadAnimation(1.f);
		PlayerController->playerDeaths++;
	}
}

void APlayerCharacter::PlayDeadAnimation(float animPlayRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, animPlayRate);
		AnimInstance->Montage_JumpToSection("Death", CombatMontage);
		GetWorldTimerManager().SetTimer(DeadTimer, this, &APlayerCharacter::StopAnim, 2.4f);
	}
}

void APlayerCharacter::StopAnim()
{
	SetActorEnableCollision(false);
	Sword->SetActorEnableCollision(false);
	Sword->EndCollision();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Pause(CombatMontage);
	}
}

void APlayerCharacter::Respawn()
{
	Destroy();
	Sword->Destroy();

	FActorSpawnParameters SpawnParameters;
	PlayerController->Possess(GetWorld()->SpawnActor<class APlayerCharacter>(SpawnThisPlayer, RespawnLocation, RespawnRotation, SpawnParameters));
}

void APlayerCharacter::Dash()
{
	if (((!GetCharacterMovement()->IsFalling())) && (ActualStaminaCharges > MinStaminaCharges) && (!bCharacterFreeze))
	{
		FVector ForwardVector = GetActorForwardVector();
		GetCharacterMovement()->BrakingFrictionFactor = 0.f;
		LaunchCharacter(FVector(ForwardVector.X, ForwardVector.Y, 0.f).GetSafeNormal() * DashDistance, true, true);
		GetWorldTimerManager().SetTimer(Timer, this, &APlayerCharacter::StopDash, 0.2f);
		bIsDashing = true;
		SetCharacterFreeze(true);
		PlayerController->playerDash++;
	}
}

void APlayerCharacter::StopDash()
{
	bIsDashing = false;
	ActualStaminaCharges--;
	if (!IsHidden())SetCharacterFreeze(false);
}

void APlayerCharacter::StaminaChargeLogic(float DrainRate)
{
	if (ActualStaminaCharges < MaxStaminaCharges)
	{
		ActualStaminaCharges += DrainRate;
	}
}

void APlayerCharacter::SetupSword()
{
	Sword = GetWorld()->SpawnActor<ASword>(SpawnThisSword);
	if (Sword)
	{
		Sword->SetCharacterAttacking(this);
		Sword->SetCombatMontage(CombatMontage);
		Sword->SetSwordMontage(SwordMontage);
		Sword->Sheath();
	}
}

void APlayerCharacter::ChangeSwordPosition()
{
	if (!IsHidden() && Sword && !Sword->bIsAttacking && !bCharacterFreeze)
	{
		SetCharacterFreeze(true);

		if (bIsSwordActive)
		{
			if (Sword)
			{
				bIsSwordActive = false;
				Sword->PlaySwordAnimation("SheathSword");
				GetWorldTimerManager().SetTimer(Timer, this, &APlayerCharacter::Sheath, 0.5f);
			}
		}
		else
		{
			if (Sword)
			{
				bIsSwordActive = true;
				Sword->PlaySwordAnimation("DrawSword");
				GetWorldTimerManager().SetTimer(Timer, this, &APlayerCharacter::Draw, 0.3f);
			}
		}
	}
}

void APlayerCharacter::Draw()
{
	Sword->Draw();
	SetCharacterFreeze(false);
}

void APlayerCharacter::Sheath()
{
	Sword->Sheath();
	SetCharacterFreeze(false);
}

void APlayerCharacter::RangeAttack()
{
	if (!bIsSwordActive && PlayerShurikens > 0 && !bCharacterFreeze)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance && CombatMontage)
		{
			AnimInstance->Montage_Play(CombatMontage, 1.f);
			AnimInstance->Montage_JumpToSection("ShurikenThrow", CombatMontage);
		}
		SetCharacterFreeze(true);
	}
}

void APlayerCharacter::SpawnShuriken()
{
	PlayerShurikens -= 1.f;
	SetCharacterFreeze(false);
	Shurikens = GetWorld()->SpawnActor<AShurikens>(SpawnThisShurikens, GetMesh()->GetSocketLocation("SwordPlacement"), FRotator(0.f, GetActorRotation().Yaw - 48.f, 0.f));
}

void APlayerCharacter::UseSword()
{
	if (!bIsDead)
	{
		if(!bIsSwordActive) ChangeSwordPosition();
		else Sword->Attack(1.f); 
	}
}

void APlayerCharacter::Interact()
{
	bIsInteracting = true;
}

void APlayerCharacter::StopInteracting()
{
	bIsInteracting = false;
}

void APlayerCharacter::MoveSideways(float Value)
{
	if (Value != 0.f && (!bCharacterFreeze))
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APlayerCharacter::MoveForward(float Value)
{
	if (Value != 0.0f && (!bCharacterFreeze))
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APlayerCharacter::Turn(float Value)
{
	if(PlayerController && !bCharacterFreeze)
	PlayerController->TurnCharacter(this);
}

void APlayerCharacter::SetupLights()
{
	for (TActorIterator<APointLight> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->IsA(APointLight::StaticClass()))
		{
			PointLightsArray.Add(*ActorItr);
		}
	}
}

float APlayerCharacter::GetLightAmount()
{
	FVector Loc = GetActorLocation();
	FCollisionQueryParams Params = FCollisionQueryParams(FName(TEXT("LightTrace")), true, this);
	float Result;

	for (int i = 0; i < PointLightsArray.Num(); i++)
	{
		FVector End = PointLightsArray[i]->GetActorLocation();
		float Distance = FVector::Dist(Loc, End);
		DistanceArray.Add(Distance);
	}

	DistanceIndex = DistanceArray.Find(FMath::Min(DistanceArray));
	DistanceArray.Empty();

	if(PointLightsArray[DistanceIndex])
	{
		UPointLightComponent* LightComp = PointLightsArray[DistanceIndex]->PointLightComponent;

		if (LightComp)
		{
			FVector End = PointLightsArray[DistanceIndex]->GetActorLocation();
			float Distance = FVector::Dist(Loc, End);
			float LightRadius = LightComp->AttenuationRadius;
			bool bHit = GetWorld()->LineTraceTestByChannel(Loc, End, ECC_Visibility, Params);

			if (Distance <= LightRadius && !bHit)
			{
				Result = FMath::Pow(FMath::Max(0.0f, 1.0f - (Distance / LightRadius)), (LightComp->LightFalloffExponent + 1)) * (LightRadius * 1.25);
				if (Result >= 5.f) Result = 5.f;

			}
		}
	}

	return Result;
}

void APlayerCharacter::BalanceHealth(float DeltaTime)
{
	float LightAmount = GetLightAmount();

	if (LightAmount > 0.01f)
	{
		PlayerHP -= DeltaTime * LightAmount;
		ActualStaminaCharges -= DeltaTime / 4;
	}
	else if (LightAmount <= 0.01f && PlayerHP < 99.f)
	{
		PlayerHP += DeltaTime * 2.f;
		ActualStaminaCharges += DeltaTime / 4;
	}
}
