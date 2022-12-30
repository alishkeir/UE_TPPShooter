// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EIemRarity:uint8
{
	EIR_Damaged UMETA(DisplayName = "Damaged"),
	EIR_Common UMETA(DisplayName = "Common"),
	EIR_Uncommon UMETA(DisplayName = "Uncommon"),
	EIR_Rare UMETA(DisplayName = "Rare"),
	EIR_Legendary UMETA(DisplayName = "Legendary"),
	EIR_MAX UMETA(DisplayName = "DEFAULTMAX")
};

UCLASS()
class TPPSHOOTER_API AItem: public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Called when start overlapping AreaSphere
	*/
	UFUNCTION()
		void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/**
	 * Called when end overlapping AreaSphere
	*/
	UFUNCTION()
		void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/**
	 * Sets the ActiveStars array of bools based on rarity
	*/
	void SetActiveStars();

public:
	//Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	/**
	 * Line trace collides with box to show hud widgets
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemProperties", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* CollisionBox;

	/**
	 * Skeletal mesh for the item
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemProperties", meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* ItemMesh;

	/**
	 * Popup widget for when the player looks at the item
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemProperties", meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* PickupWidget;

	/**
	 * Enables item tracing when overl;apped
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemProperties", meta = (AllowPrivateAccess = "true"))
		class USphereComponent* AreaSphere;

	/**
	 * The name which appears on the PickupWidget
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemProperties", meta = (AllowPrivateAccess = "true"))
		FString ItemName;

	/**
	 * The count of ammo which appears on the PickupWidget
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemProperties", meta = (AllowPrivateAccess = "true"))
		int32 ItemCount;

	/**
	 * Rarity of the item - determines number of starts in PickupWidget
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemProperties", meta = (AllowPrivateAccess = "true"))
		EIemRarity ItemRarity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemProperties", meta = (AllowPrivateAccess = "true"))
		TArray<bool> ActiveStars;
public:

	FORCEINLINE UWidgetComponent* GetPickupWidget() const { return PickupWidget; };

	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; };

	FORCEINLINE UBoxComponent* GetCollisionBox() const { return CollisionBox; };
};
