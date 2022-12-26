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

	/// Ranzomized gunshot sound cue
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		class USoundCue* FireSound;

	/// Flash spawned at BarrelSocket
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		class UParticleSystem* MuzzleFlash;

	/// Montage for firing the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* HipFireMontage;

	/// Particles spawn upon bullet impact
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		UParticleSystem* ImpactParticles;

	/// Smoke trail for bullets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		UParticleSystem* BeamParticles;

	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation);

	/// true when aiming
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		bool bAiming;

	/// Default Camera Field Of View Value
	float CameraDefaultFOV;

	/// Zoomed Camera Field Of View Value
	float CameraZoomedFOV;

	/// Current Field Of View this frame
	float CameraCurrentFOV;

	/// Inter speed for zooming when aiming
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		float ZoomInterpSpeed;

	/// handle interpolation for zoom when aiming
	void CameraInterpZoom(float DeltaTime);

	/// turn rate while not aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		float HipTurnRate;

	/// lookup rate while not aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		float HipLooupRate;

	/// turn rate while aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		float AimingTurnRate;

	/// lookup rate while aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		float AimingLookupRate;

	/// Scale factor for mouse look sensitivity. Turn rate when not aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float MouseHipTurnRate;

	/// Scale factor for mouse lookup sensitivity. Turn rate when not aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float MouseAimingTurnRate;

	/// Scale factor for mouse look sensitivity. Turn rate when aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float MouseHipLookupRate;

	/// Scale factor for mouse lookup sensitivity. Turn rate when aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float MouseAimingLookupRate;

	/// Detertmine the spread of the crosshair
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crossharis", meta = (AllowPrivateAccess = "true"))
		float CrosshairSpreadMultiplier;

	/// Velocity component for crosshair spread
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crossharis", meta = (AllowPrivateAccess = "true"))
		float CrosshairVelocityFactor;

	/// In air component for crosshair spread
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crossharis", meta = (AllowPrivateAccess = "true"))
		float CrosshairInAirFactor;

	/// Aim component for crosshair spread
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crossharis", meta = (AllowPrivateAccess = "true"))
		float CrosshairAimFactor;

	/// Shooting component for crosshair spread
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crossharis", meta = (AllowPrivateAccess = "true"))
		float  CrosshairShootingFactor;

	float ShootTimeDuration;

	bool bFiringBullet;

	FTimerHandle CrosshairShootTimer;

	/// Assigned fire button pressed
	bool bFireButtonPressed;

	/// True ehdn we can fire -- false whern waiting for the timer
	bool bShouldFire;

	/// Rate of automatic gun fire
	float AutomaticFireRate;

	/// Sets a timer between gun shots
	FTimerHandle AutoFireTimer;

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

	/// Set bAiming to true
	void AimingButtonPressed();

	/// Set bAiming to false
	void AimingButtonReleased();

	/// Set BaseTurnRate and BaseLookupRates based on aiming
	void SetLookRates();

	/**
	 * @brief Rotate controller based on mouse X movement
	 * @param Value The input  value from the mouse movement
	*/
	void Turn(float Value);

	/**
	* @brief Rotate controller based on mouse Y movement
	* @param Value The input  value from the mouse movement
	*/
	void Lookup(float Vale);

	void CalculateCrosshairSpread(float DeltaTime);

	void StartCrosshairBulletFire();

	UFUNCTION()
		void FinishCrosshairBulletFire();

	void FireButtonPressed();

	void FireButtonReleased();

	void StartFireTimer();

	UFUNCTION()
		void AutoFireReset();

	/**
	 * @brief check if the crosshair is pointing at the weapon location
	 * @param OutHitResult GG
	 * @param OutHitLocation
	 * @return true or false
	*/
	bool TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation);

public:

	/// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// Returns CameraBoom subobject
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/// Returns FollowCamera subobject
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void MoveForward(float Value); /// called for forward/backward inputs

	void MoveRight(float Value); /// called for right/left inputs

	FORCEINLINE bool GetAiming() const { return bAiming; }

	UFUNCTION(BlueprintCallable)
		float GetCrosshairSpreadMultiplier() const;
};
