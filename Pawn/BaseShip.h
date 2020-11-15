// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//Engine
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

//ZeroG
#include "ZeroG/Components/HealthManager.h"
#include "ZeroG/Game/ZeroGInterface.h"
#include "ZeroG/Game/Weapon/WeaponManager.h"

#include "BaseShip.generated.h"

UCLASS()
class ZEROG_API ABaseShip : public APawn, public IZeroGInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION(BlueprintCallable, Category = "Controll")
	void AddThrust(const FVector Direction, const float InputStrength) const;

	UFUNCTION(BlueprintCallable, Category = "Controll")
	void AddRotation(const FVector Axis) const;

public:
	
	//Character Input
	UFUNCTION(BlueprintCallable, Category = "Controll")
	void Yaw(const float Input);
	UFUNCTION(BlueprintCallable, Category = "Controll")
	void Pitch(const float Input);
	UFUNCTION(BlueprintCallable, Category = "Controll")
	void Roll(const float Input);

	UFUNCTION(BlueprintCallable, Category = "Controll")
	void Surge(const float Input);
	UFUNCTION(BlueprintCallable, Category = "Controll")
	void Sway(const float Input);
	UFUNCTION(BlueprintCallable, Category = "Controll")
	void Heave(const float Input);

	UFUNCTION(BlueprintCallable, Category = "Controll")
	void CycleTarget();

	
	UFUNCTION()
	virtual void OnSendDeath() override;

	UFUNCTION()
	void DestroyShip();

	
public:


	



	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ShipModel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UWeaponManager* WeaponManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UHealthManager* HealthManager;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ThrustStrength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotationStrength;

	UPROPERTY(EditAnywhere)
	float BackWardStrength;
	
	UPROPERTY(EditAnywhere)
	float SurgeStrength;

	UPROPERTY(EditAnywhere)
	float SwayStrength;

	UPROPERTY(EditAnywhere)
	float HeaveStrength;

	UPROPERTY(EditAnywhere)
	float ScanArea;
	
	/**
	* Particle which should spawn when the ship explodes.
	*/
	UPROPERTY(EditAnywhere)
	UParticleSystem* ExplosionParticle;
	
private:
	UPROPERTY()
	int LastRandomTarget;


	
};
