// Fill out your copyright notice in the Description page of Project Settings.


#include "NinjaAnimInstance.h"
#include "PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

void UNinjaAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}
	if (Pawn)
	{
		Player = Cast<APlayerCharacter>(Pawn);
	}
}

void UNinjaAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}
	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();

		bIsInAir = Pawn->GetMovementComponent()->IsFalling();
		Player = Cast <APlayerCharacter>(Pawn);
		
		if (Player)
		{
			bIsCrouching = Player->bIsCrouching;
			bIsDashing = Player->bIsDashing;
			bIsSwordActive = Player->bIsSwordActive;
		}
	}
}