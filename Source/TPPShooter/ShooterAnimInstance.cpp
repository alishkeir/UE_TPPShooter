// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterAnimInstance.h"
#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UShooterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (ShooterCharacter == nullptr)
	{
		ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
	}

	if (ShooterCharacter)
	{
		/// get speed of character from velocity
		FVector Velocity{ShooterCharacter->GetVelocity()};
		Velocity.Z = 0;
		Speed = Velocity.Size();

		/// is the character in the air
		bIsInAir = ShooterCharacter->GetCharacterMovement()->IsFalling();

		/// is the character accelerating
		if (ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
		{
			bIsAccelerating = true;
		} else
		{
			bIsAccelerating = false;
		}

		FRotator AimingRotation = ShooterCharacter->GetBaseAimRotation();

		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(ShooterCharacter->GetVelocity());

		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimingRotation).Yaw;

		/*	FString RotationMessage = FString::Printf(TEXT("BaseAimRotation: %f"), AimingRotation.Yaw);
			FString MovementRotationMessage = FString::Printf(TEXT("\n MovementRotation: %f"), MovementRotation.Yaw);
			*/

			//FString OffsetMessage = FString::Printf(TEXT("MovementOffsetYaw: %f"), MovementOffsetYaw);
			//if (GEngine)
			//{
			//	/*
			//	GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::White, RotationMessage);
			//	GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Red, MovementRotationMessage);
			//	*/
			//	GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Red, OffsetMessage);
			//}
	}
}

void UShooterAnimInstance::NativeInitializeAnimation()
{
	ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
}