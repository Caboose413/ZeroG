// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeaponSlot.h"
#include "ZeroG/Game/Weapon/BaseWeapon.h"

UBaseWeaponSlot::UBaseWeaponSlot()
{
    
}

//Function to forward out Shoot command to the Actual Weapon.
void UBaseWeaponSlot::ShootSlot(bool Shooting)
{
    Cast<ABaseWeapon>(GetChildActor())->ShootWeapon(Shooting);
}
