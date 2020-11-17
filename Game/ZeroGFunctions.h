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

	UPROPERTY()
	float DeltaTime;
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
			if (!Wep)
			{
				return;
			}
			FVector TargetNormal = FVector();

			if (Info.TargetActor)
			{	
				switch (Wep->GimbalType)
				{
					case Static:
					{
						//For static weapons we only want the weapons to align to the potential hit location.

						//Get the Distance between the Weapons Owner and the target.
						const float TargetDist = FVector::Dist(Info.ShipLocation, Info.TargetActor->GetActorLocation());

						//Make an Aim location from the Target Distance and our Ships forward Vector.
						const FVector AimLocation = Info.ShipLocation + (Info.ShipForward * TargetDist);

						//Subtract the Aim location from the Weapons location so we can produce a normal.
						TargetNormal = (AimLocation - Wep->GetComponentLocation()).GetSafeNormal();
						
						Info.CursorLocation = AimLocation;
						//UE_LOG(LogTemp, Warning, TEXT("Dot %f"),TargetDist);
						break;
					};
					case Gimbal:
					{
						//Todo add a move able mouse cursor so we can gimbal the weapon towards it. 
						break;
					};
					case Auto:
					{
						//For auto weapon we want the weapon to aim at a predicted Location of where the target will be.

						//Get a pointer to the actual weapon so we can access some information in it.
						ABaseWeapon* Weapon = Cast<ABaseWeapon>(Wep->GetChildActor());

						//Calculate the distance from the Owner to the Target.
						const float PredictionDistance = FVector::Dist(Weapon->SpawnLoc->GetComponentLocation(), Info.TargetActor->GetActorLocation());

						//Calculate the Time it takes to reach the distance.
						const float HitTime = PredictionDistance / Weapon->ProSpeed;

						//Take the Owner and Targets velocity into account.
						const FVector VelocityDifference = Info.TargetActor->GetVelocity() - Wep->GetOwner()->GetVelocity();

						//Make the final Predicted Location on where the Target ship should be at the time of impact.
						const FVector PredictedLocation = Info.TargetActor->GetActorLocation() + (VelocityDifference * HitTime);

						//Make a normal from our projectile spawn point to the Predicted Location.
						TargetNormal = (PredictedLocation - Weapon->SpawnLoc->GetComponentLocation()).GetSafeNormal();
						break;
					};
				}
			}
			else
			{
				if (Info.TraceHit.bBlockingHit)
				{
					Info.CursorLocation = Info.TraceHit.Location;
					TargetNormal = (Info.TraceHit.Location - Wep->GetComponentLocation()).GetSafeNormal();
				}
				else
				{
					Info.CursorLocation = Info.TraceHit.TraceEnd;
					TargetNormal = (Info.TraceHit.TraceEnd - Wep->GetComponentLocation()).GetSafeNormal();
				}
			}
			//Set the Calculated Rotation.
			Wep->SetWorldRotation(TargetNormal.Rotation());
		}
	}
};


UCLASS()
class ZEROG_API UZeroGFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	static FVector GetAimPrediction(AActor* Owner, AActor* Target, float Velocity);
};
