// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponManager.h"
#include "ZeroG/Game/Weapon/BaseWeapon.h"

// Sets default values for this component's properties
UWeaponManager::UWeaponManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeaponManager::BeginPlay()
{
	Super::BeginPlay();

	//Get All Weapon Slots attached to the Ship.
	TArray<UActorComponent*> ShipWeaponSlots;
	GetOwner()->GetComponents(ShipWeaponSlots);

	//Loop over All Weapon Slots.
	for (auto& Weapon : ShipWeaponSlots)
	{
		//Cast to the Actual Class.
		UBaseWeaponSlot* ActualSlot = Cast<UBaseWeaponSlot>(Weapon);
		if (ActualSlot)
		{
			//Register the Owning ship to the Weapon.
			AddWeapon(ActualSlot->Group, ActualSlot);
		}
	}
}

// Called every frame
void UWeaponManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	const FVector OwnerLoc = GetOwner()->GetActorLocation();
	const FVector OwnerForward = GetOwner()->GetActorForwardVector();
	const FVector LineStart = OwnerForward * 1000.0f + OwnerLoc;
	const FVector LineEnd = OwnerLoc + (OwnerForward * 900000.0f);

	FHitResult LineHit;
	GetWorld()->LineTraceSingleByChannel(LineHit, LineStart, LineEnd, ECC_Visibility);

	if (LineHit.bBlockingHit)
	{
		AimLocation = LineHit.Location;
	}
	else
	{
		AimLocation = LineEnd;
	}
	
	for (auto& Weapon : WeaponGroup)
	{
		Weapon.SetRotation(AimLocation);
	}
	
	// ...
}

void UWeaponManager::SetWeapon(int index, FString Group, TArray<UBaseWeaponSlot*> Weapons)
{
	WeaponGroup.EmplaceAt(index, FWeaponGroup(Group, Weapons));
}

//Adds a Weapon to a Certain group and Registers the Owning ship onto the Weapon.
void UWeaponManager::AddWeapon(FString Group, UBaseWeaponSlot* Weapon)
{
	//Chekcs if the Group already exist.
	if (HasWeaponGroup(Group))
	{
		//Adds a New weapon to the Group.
		GetWeaponGroup(Group).AddWeapon(Weapon);
	}
	else
	{
		//Create a New Group and add a Weapon to it.
		WeaponGroup.Add(FWeaponGroup(Group, Weapon));
	}

	//Register the Ship owner onto the Weapon.
	Cast<ABaseWeapon>(Weapon->GetChildActor())->SetOwningShip(GetOwner());
}

//Gets a Reference to the Weapon Group.
FWeaponGroup& UWeaponManager::GetWeaponGroup(FString GroupToSearch)
{
	for (auto& TheGroup : WeaponGroup)
	{
		if (TheGroup.Group == GroupToSearch)
		{
			return TheGroup;
		}
	}
	return EmptyGroup;
}

//Checks if a Certain group already Exists.
bool UWeaponManager::HasWeaponGroup(FString Group)
{
	for (auto& TheGroup : WeaponGroup)
	{
		if (TheGroup.Group == Group)
		{
			return true;
		}
	}
	return false;
}

//Send fire command to whole Group.
void UWeaponManager::ExeShoot(FString Group, bool Shooting)
{
	for (auto& TheGroup : WeaponGroup)
	{
		//Checks if the Group matches the Requested Group which should fire.
		if (TheGroup.Group == Group)
		{
			//Execute Shoot function on the Struct.
			TheGroup.ShootGroup(Shooting);
		}
	}
}

