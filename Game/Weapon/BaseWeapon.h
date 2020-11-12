// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//Engine
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"

//ZeroG
#include "Projectile/BaseProjectile.h"
#include "BaseWeapon.generated.h"

UCLASS()
class ZEROG_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	void ShootWeapon(bool Shooting);

	void ExeShoot();

	void SetOwningShip(AActor* NewOwner);
	
	UPROPERTY(EditAnywhere)
	float RPM;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseProjectile> Projectile;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootScene;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* WepMesh;
	
	UPROPERTY(VisibleAnywhere)
	UArrowComponent* SpawnLoc;

	
private:
	FTimerHandle ShootTimer;
};
