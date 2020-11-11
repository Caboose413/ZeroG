// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//Engine
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

//ZeroG
#include "ZeroG/Game/ZeroGFunctions.h"
#include "WeaponManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZEROG_API UWeaponManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:

	UFUNCTION()
	void SetWeapon(int index, FString Group, TArray<UBaseWeaponSlot*> Weapons);

	UFUNCTION()
	void AddWeapon(FString Group, UBaseWeaponSlot* Weapon);
	
	UFUNCTION()
	FWeaponGroup& GetWeaponGroup(FString Group);
	
	UFUNCTION()
	bool HasWeaponGroup(FString Group);

	UFUNCTION(BlueprintCallable)
	void ExeShoot(FString Group, bool Shooting);

private:

	UPROPERTY()
	TArray<FWeaponGroup> WeaponGroup;
	
	FWeaponGroup EmptyGroup;
};
