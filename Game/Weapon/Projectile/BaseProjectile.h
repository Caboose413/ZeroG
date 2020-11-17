// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//Engine
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"

//ZeroG
#include "BaseProjectile.generated.h"

UCLASS()
class ZEROG_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	/**
	* Function to Call the Destruction of our Particle.
	* @note we call this insteal off Destroy() so we can play certain effects before the actually destroying the Actor.
	*/
	UFUNCTION()
	void DestroyProjectile();

	/** 
	* Event when this actor bumps into a blocking object, or blocks another actor that bumps into it.
	* This could happen due to things like Character movement, using Set Location with 'sweep' enabled, or physics simulation.
	* For events when objects overlap (e.g. walking into a trigger) see the 'Overlap' event.
	*
	* @note For collisions during physics simulation to generate hit events, 'Simulation Generates Hit Events' must be enabled.
	* @note When receiving a hit from another object's movement (bSelfMoved is false), the directions of 'Hit.Normal' and 'Hit.ImpactNormal'
	* will be adjusted to indicate force from the other object against this object.
	*/
	UFUNCTION()
	void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	void ActivateCollision();

	
	/**
	* Actor's Weapon Model.
	*/
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ProMesh;

	/**
	* Actor's Collision Model.
	*/
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* ProCol;

	/**
	* Projectile Movement Component to handle the movement of the Actor.
	*/
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProMovement;

	/**
	* The Amount of Damage the Projectile should apply.
	*/
	UPROPERTY(EditAnywhere)	
	TSubclassOf<UDamageType> ProDamageType;
	
	/**
	* Particle which should spawn if the Projectile hit's a target.
	*/
	UPROPERTY(EditAnywhere)
	UParticleSystem* HitParticle;

	/**
	* The Amount of Damage the Projectile should apply.
	*/
	UPROPERTY(EditAnywhere)
	float Damage;

	UPROPERTY(EditAnywhere)
	float LifeTime;
};


