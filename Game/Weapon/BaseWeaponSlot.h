// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//Engine
#include "CoreMinimal.h"
#include "Components/ChildActorComponent.h"

//ZeroG
#include "BaseWeaponSlot.generated.h"

/**
 * Class to dynamically Switch out Weapons on a Slot.
 */
UCLASS(Blueprintable)
class ZEROG_API UBaseWeaponSlot : public UChildActorComponent
{
	GENERATED_BODY()

public:
    UBaseWeaponSlot();

    UFUNCTION()
    void ShootSlot(bool Shooting);

    UPROPERTY(EditAnywhere)
    FString Group;
};
