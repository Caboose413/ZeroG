// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//Engine
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

//ZeroG
#include "ZeroG/Game/Weapon/BaseWeaponSlot.h"
#include "ZeroGFunctions.generated.h"

/**
 * 
 */

//Struct to Handle our Groups and weapons with them.
USTRUCT(BlueprintType)
struct FWeaponGroup
{
	GENERATED_USTRUCT_BODY()

	//The Group if this Weapon Array.
	UPROPERTY()
	FString Group;

	//Array of Weapons.
	UPROPERTY(BlueprintReadWrite)
	TArray<UBaseWeaponSlot*> WeaponSlots;

	//Default Constructors.
	FWeaponGroup()
	{
		Group = "none";
		WeaponSlots.AddDefaulted(0);
	};
	
	FWeaponGroup(FString theGroup, TArray<UBaseWeaponSlot*> theWeapons)
	{
		Group = theGroup;
		WeaponSlots = theWeapons;
	};

	FWeaponGroup(FString theGroup, UBaseWeaponSlot* theWeapon)
	{
		Group = theGroup;
		WeaponSlots.Add(theWeapon);
	};

	//Adds a Weapon to the Struct.
	void AddWeapon(UBaseWeaponSlot* WeaponToAdd)
	{
		WeaponSlots.Add(WeaponToAdd);	
	}

	//Call shoot function on all weapons in the Struct.
	void ShootGroup(bool Shooting)
	{
		for(auto& Wep : WeaponSlots)
		{
			Wep->ShootSlot(Shooting);
		}
	}
	void SetRotation(FVector AimLocation)
	{
		for(auto& Wep : WeaponSlots)
		{
			FVector AimNormal = (AimLocation - Wep->GetComponentLocation()).GetSafeNormal();
			Wep->SetWorldRotation(AimNormal.Rotation());
		}
	}
};


UCLASS()
class ZEROG_API UZeroGFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
