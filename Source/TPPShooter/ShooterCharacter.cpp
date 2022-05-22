// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/SkeletalMeshSocket.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter():
	BaseTurnRate(45.f),
	BaseLookupRate(45.f)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom")); /// create camera boom
	CameraBoom->SetupAttachment(RootComponent); /// attach to root compnent
	CameraBoom->TargetArmLength = 300.f; /// camera follow the player at this value
	CameraBoom->bUsePawnControlRotation = true; /// rotate the arm based on the controller
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 50.f);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera")); /// create follow camera
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); /// attach camera to spring arm
	FollowCamera->bUsePawnControlRotation = false; /// we only need the camera to follow CameraBoom, not to rotate with controller

	/// Don't rotate character with controller rotating
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false; /// character can rotate to Yaw controller rotation
	bUseControllerRotationRoll = false;

	/// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; /// character does not turn into the direction of the input ...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); /// ... at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);

	PlayerInputComponent->BindAxis("TurnRate", this, &AShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterCharacter::LookAtRate);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("FireButton", IE_Pressed, this, &AShooterCharacter::FireWeapon);
	//PlayerInputComponent->BindAction("FireButton", IE_Released, this, &ACharacter::StopJumping);
}

void AShooterCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		/// find out which way si forward

		const FRotator Rotation{Controller->GetControlRotation()};
		const FRotator YawRotation{0, Rotation.Yaw, 0};

		const FVector Direction{FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X)};

		AddMovementInput(Direction, Value);
	}
}

void AShooterCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		/// find out which way si forward

		const FRotator Rotation{Controller->GetControlRotation()};
		const FRotator YawRotation{0, Rotation.Yaw, 0};

		const FVector Direction{FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y)};

		AddMovementInput(Direction, Value);
	}
}

void AShooterCharacter::TurnAtRate(float Rate)
{
	/// Calulate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookupRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::FireWeapon()
{
	//UE_LOG(LogTemp, Warning, TEXT("FireWeapon"));
	if (FireSound)
	{
		UGameplayStatics::PlaySound2D(this, FireSound);
	}

	const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName("BarrelSocket");

	if (BarrelSocket)
	{
		const FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetMesh());

		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
		}

		/// Get current viewport size
		FVector2D ViewportSize;

		if (GEngine && GEngine->GameViewport)
		{
			GEngine->GameViewport->GetViewportSize(ViewportSize);
		}

		/// get screen space location of crosshairs
		FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f); /// get criosshair location
		CrosshairLocation.Y -= 30.f; /// decrease the size of the Y Axis as we set in our blueprints

		FVector CrosshairWorldPosition;
		FVector CrosshairWorldDirection;

		/// get world position and direction of crosshairs
		bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);

		/// was dprojection successful
		if (bScreenToWorld)
		{
			FHitResult ScreenTraceHit;

			const FVector Start{CrosshairWorldPosition};

			const FVector End{CrosshairWorldPosition + CrosshairWorldDirection * 50'000};

			/// set beam endpoint to line trace endpoint
			FVector BeamEndPoint{End};

			/// trace outwards from crosshairs world location
			GetWorld()->LineTraceSingleByChannel(ScreenTraceHit, Start, End, ECollisionChannel::ECC_Visibility);

			/// was there a trace hit
			if (ScreenTraceHit.bBlockingHit)
			{
				BeamEndPoint = ScreenTraceHit.Location; /// BeamEndPoint is now ScreenTraceHit Location
			}

			if (ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, ScreenTraceHit.Location);
			}

			if (BeamParticles)
			{
				UParticleSystemComponent* Beam{UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamParticles, SocketTransform)};
				Beam->SetVectorParameter(FName("Target"), BeamEndPoint); ///  set the target of the beam from "Target Name" parameter in the target section of the beam particles system
			}
		}

		//FHitResult FireHit;

		//const FVector Start{SocketTransform.GetLocation()}; /// get start point ast the socket location on the weapon muzzle

		//const FQuat Rotation{SocketTransform.GetRotation()}; /// get rotation in 3d space

		//const FVector RotationAxis{Rotation.GetAxisX()}; /// get X-Axis rotation in 3d space

		//const FVector End{Start + RotationAxis * 50'000.f};

		//FVector BeamEndPoint{End};

		//GetWorld()->LineTraceSingleByChannel(FireHit, Start, End, ECollisionChannel::ECC_Visibility);

		//if (FireHit.bBlockingHit)
		//{
		//	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f); ///  draw red line from the weapon muzzle to max of 50,000 unit to the front (can be lower if it hits something)
		//	//DrawDebugPoint(GetWorld(), FireHit.Location, 5.f, FColor::Red, false, 2.f); /// draw a point on the hit location

		//	BeamEndPoint = FireHit.Location; /// set beam end point to the hit point location

		//	if (ImpactParticles)
		//	{
		//		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, FireHit.Location);
		//	}
		//}

		//if (BeamParticles)
		//{
		//	UParticleSystemComponent* Beam{UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamParticles, SocketTransform)};
		//	Beam->SetVectorParameter(FName("Target"), BeamEndPoint); ///  set the target of the beam from "Target Name" parameter in the target section of the beam particles system
		//}
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && HipFireMontage)
	{
		AnimInstance->Montage_Play(HipFireMontage);
		AnimInstance->Montage_JumpToSection(FName("StartFire"));
	}
}