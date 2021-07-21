// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"
#include "PlayerCharacter.h"
#include "Enemy.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"

#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"


#include "TimerManager.h"

// Sets default values
ASword::ASword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	SwordMesh->SetupAttachment(GetRootComponent());

	SwordCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordCollision"));
	SwordCollision->SetupAttachment(SwordMesh);

	AttackCounter = 0.f;
	TimerAttack = 5.f;
	Damage = 25.f;

	bIsAttacking = false;
}

// Called when the game starts or when spawned
void ASword::BeginPlay()
{
	Super::BeginPlay();

	SwordCollision->OnComponentBeginOverlap.AddDynamic(this, &ASword::OnBeginOverlap);
	SwordCollision->OnComponentEndOverlap.AddDynamic(this, &ASword::OnEndOverlap);

	SwordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SwordCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	SwordCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SwordCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

// Called every frame
void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsAttacking) StopAttackingAfterAnim();
}

void ASword::AddToCounter()
{
	AttackCounter++;
}

void ASword::ResetCounter()
{
	AttackCounter = 0.f;
}

void ASword::Draw()
{
		const USkeletalMeshSocket* HandSocket = CharacterAttacking->GetMesh()->GetSocketByName("SwordPlacement");
		if (HandSocket)
		{
			SetActorScale3D(HandSocket->GetSocketTransform(CharacterAttacking->GetMesh()).GetScale3D());
			HandSocket->AttachActor(this, CharacterAttacking->GetMesh());
		}
}

void ASword::Sheath()
{
		SwordMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		SwordMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		SwordMesh->SetSimulatePhysics(false);

		const USkeletalMeshSocket* BackSocket = CharacterAttacking->GetMesh()->GetSocketByName("SwordHolder");
		if (BackSocket)
		{
			SetActorScale3D(BackSocket->GetSocketTransform(CharacterAttacking->GetMesh()).GetScale3D());
			BackSocket->AttachActor(this, CharacterAttacking->GetMesh());
		}
}

void ASword::Attack(float animPlayRate)
{
	if (!bIsAttacking)
	{
		if (AttackCounter > 2.f)
		{
			PlayAttackAnimation(animPlayRate);
			if (Cast<class APlayerCharacter>(CharacterAttacking))
			{
				ResetCounter();
				Cast<class APlayerCharacter>(CharacterAttacking)->bCharacterFreeze = true;
				AddToCounter();
			}
		}
		else
		{
			PlayAttackAnimation(animPlayRate);
			if (Cast<class APlayerCharacter>(CharacterAttacking))
			{
				Cast<class APlayerCharacter>(CharacterAttacking)->bCharacterFreeze = true;
				AddToCounter();
			}
		}
		bIsAttacking = true;
		GetWorldTimerManager().SetTimer(CounterResetTimer, this, &ASword::ResetCounter, TimerAttack);
	}
}

void ASword::PlaySwordAnimation(FName SectionName)
{
	if (CharacterAttacking)
	{
		UAnimInstance* AnimInstance = CharacterAttacking->GetMesh()->GetAnimInstance();

		if (AnimInstance && SwordMontage)
		{
			AnimInstance->Montage_Play(SwordMontage, 1.f);
			AnimInstance->Montage_JumpToSection(SectionName, SwordMontage);

			if (SectionName == "DrawSword") GetWorldTimerManager().SetTimer(AnimationTimer, this, &ASword::Draw, 0.3f);
			else if (SectionName == "SheathSword") GetWorldTimerManager().SetTimer(AnimationTimer, this, &ASword::Sheath, 0.5f);
		}
	}
}

void ASword::PlayAttackAnimation(float animPlayRate)
{
	if (CharacterAttacking)
	{
		UAnimInstance* AnimInstance = CharacterAttacking->GetMesh()->GetAnimInstance();

		if (AnimInstance && CombatMontage)
		{
			AnimInstance->Montage_Play(CombatMontage, animPlayRate);
			if (AttackCounter == 0.f)
			{
				AnimInstance->Montage_JumpToSection("AttackUpper", CombatMontage);
			}
			else if (AttackCounter == 1.f)
			{
				AnimInstance->Montage_JumpToSection("AttackDown", CombatMontage);
			}
			else if (AttackCounter == 2.f)
			{
				AnimInstance->Montage_JumpToSection("AttackKick", CombatMontage);
			}
		}
	}
}

void ASword::StopAttackingAfterAnim()
{
	if (CharacterAttacking)
	{
		UAnimInstance* AnimInstance = CharacterAttacking->GetMesh()->GetAnimInstance();
		
		if (AnimInstance)
		{
			if (AnimInstance->Montage_GetBlendTime(CombatMontage) == 0.f)
			{
				bIsAttacking = false;
				if (Cast<class APlayerCharacter>(CharacterAttacking))
				{
					Cast<class APlayerCharacter>(CharacterAttacking)->bCharacterFreeze = false;
				}
			}
		}
	}
}

void ASword::ActivateCollision()
{
	SwordCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ASword::EndCollision()
{
	SwordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASword::OnBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		if(Cast<class APlayerCharacter>(CharacterAttacking)) 
		{
			if (Cast<AEnemy>(OtherActor))
			{
				AEnemy* Enemy = Cast<AEnemy>(OtherActor);
				Enemy->EnemyHealth -= Damage;
				if (!Cast<class APlayerCharacter>(Enemy->TargetToFollow))
				{
					Enemy->TargetToFollow = Cast<class APlayerCharacter>(CharacterAttacking);
					Enemy->MoveToTarget(Cast<class APlayerCharacter>(CharacterAttacking));
				}
			}
		}
		if(Cast<class AEnemy>(CharacterAttacking))
		{
			if (Cast<class APlayerCharacter>(OtherActor))
			{
				Cast<class APlayerCharacter>(OtherActor)->PlayerHP -= Damage;
			}
		}
	}
}

void ASword::OnEndOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

}
