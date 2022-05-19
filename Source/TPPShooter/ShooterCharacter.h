// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class TPPSHOOTER_API AShooterCharacter: public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * @brief  called via input to turn at given rate
	 * @param Rate this is a normalized rate, i.e. 1.0 means 100% of the desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* @brief  called via input to looktp/lookdown at given rate
	* @param Rate this is a normalized rate, i.e. 1.0 means 100% of the desired look rate
	*/
	void LookAtRate(float Rate);

	/* called when the Fire Button is pressed */
	void FireWeapon();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	/// CameraBoom positioning the camera behind the chartacter
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/// Camera that follows the chartacter
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	/// Base turn Rate in Degrees/Second ... other scaling may affect final turn rate
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		float BaseTurnRate;

	/// Base lookup/lookdown Rate in Degrees/Second ... other scaling may affect final look rate
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		float BaseLookupRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		class USoundCue* FireSound;

public:

	/// Returns CameraBoom subobject
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/// Returns FollowCamera subobject
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void MoveForward(float Value); /// called for forward/backward inputs

	void MoveRight(float Value); /// called for right/left inputs
};
