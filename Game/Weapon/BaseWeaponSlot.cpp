// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeaponSlot.h"
#include "ZeroG/Game/Weapon/BaseWeapon.h"

UBaseWeaponSlot::UBaseWeaponSlot()
{
    PrimaryComponentTick.bCanEverTick = true;

    Group = TEXT("default");
    GimbalType = Static;

    MaxGimbal = 10.0f;
}

void UBaseWeaponSlot::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

//Function to forward out Shoot command to the Actual Weapon.
void UBaseWeaponSlot::ShootSlot(bool Shooting)
{
    Cast<ABaseWeapon>(GetChildActor())->ShootWeapon(Shooting);
}



