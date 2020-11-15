// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//Engine
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

//ZeroG
#include "Weapon/BaseWeapon.h"
#include "ZeroG/Game/Weapon/BaseWeaponSlot.h"
#include "ZeroGFunctions.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FWeaponInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FVector CursorLocation;
	
	UPROPERTY()
	FVector ShipLocation;

	UPROPERTY()
	FVector ShipForward;
	
	UPROPERTY()
	AActor* TargetActor;
	
	UPROPERTY()
	FHitResult TraceHit;
};

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
	void SetRotation(FWeaponInfo& Info)
	{
		for(auto& Wep : WeaponSlots)
		{
			FVector TargetNormal;

			if (Info.TargetActor)
			{	
				switch (Wep->GimbalType)
				{
				case Static:
					{
						if (!Wep)
						{
							return;
						}
						const float TargetDist = FVector::Dist(Info.ShipLocation, Info.TargetActor->GetActorLocation());
						const FVector AimLocation = Info.ShipLocation + (Info.ShipForward * TargetDist);
						TargetNormal = (AimLocation - Wep->GetComponentLocation()).GetSafeNormal();
						Info.CursorLocation = AimLocation;
						//UE_LOG(LogTemp, Warning, TEXT("Dot %f"),TargetDist);
						break;
					};
				case Gimbal:
					{
	                   break;
					};
				case Auto:
					{
						if (!Wep)
						{
							return;
						}
						ABaseWeapon* Weapon = Cast<ABaseWeapon>(Wep->GetChildActor());
							
						const float PredictionDistance = FVector::Dist(Weapon->SpawnLoc->GetComponentLocation(), Info.TargetActor->GetActorLocation()) / (Weapon->ProSpeed);

						const FVector VelocityDifference = Info.TargetActor->GetVelocity() - Weapon->GetOwner()->GetVelocity();

						const FVector PredictedLocation = (VelocityDifference * PredictionDistance) + Info.TargetActor->GetActorLocation();

						TargetNormal = (PredictedLocation - Weapon->SpawnLoc->GetComponentLocation()).GetSafeNormal();
						break;
					};
				}
			}
			else
			{
				if (Info.TraceHit.bBlockingHit)
				{
					if (!Wep)
					{
						return;
					}
					Info.CursorLocation = Info.TraceHit.Location;
					TargetNormal = (Info.TraceHit.Location - Wep->GetComponentLocation()).GetSafeNormal();
				}
				else
				{
					if (!Wep)
					{
						return;
					}
					Info.CursorLocation = Info.TraceHit.TraceEnd;
					TargetNormal = (Info.TraceHit.TraceEnd - Wep->GetComponentLocation()).GetSafeNormal();
				}
			}
			Wep->SetWorldRotation(TargetNormal.Rotation());
		}
	}
};


UCLASS()
class ZEROG_API UZeroGFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION()
	static FVector GetAimPrediction(AActor* Owner, AActor* Target, float Velocity);
};
