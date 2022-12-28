// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

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
public:

	FORCEINLINE UWidgetComponent* GetPickupWidget() const { return PickupWidget; };
};
