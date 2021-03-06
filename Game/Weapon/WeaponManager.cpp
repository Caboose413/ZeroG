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
	RefreshWeaponData();
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

void UWeaponManager::RefreshWeaponData()
{
	for (auto& TheGroup : WeaponGroup)
	{
		TheGroup.SetRotation(GetWeaponInfo());
	}

	AimLocation = GetWeaponInfo().CursorLocation;
}

FWeaponInfo& UWeaponManager::GetWeaponInfo()
{
	const AActor* OwningShip = GetOwner();
	if (!OwningShip)
	{
		return WepInfo;
	}

	//Prepare Data for the Weapon info.
	const FVector OwnerForward = GetOwner()->GetActorForwardVector();
	const FVector OwnerLocation = GetOwner()->GetActorLocation();
	const FVector LineStart = OwnerForward * 1000.0f + OwnerLocation;
	const FVector LineEnd = OwnerLocation + (OwnerForward * 900000.0f);

	//Construct Weapon info.
	WepInfo.ShipLocation = OwnerLocation;
	WepInfo.ShipForward = OwnerForward;
	WepInfo.DeltaTime = GetWorld()->GetDeltaSeconds();
	GetWorld()->LineTraceSingleByChannel(WepInfo.TraceHit, LineStart, LineEnd, ECC_Visibility);

	//Return if there is no target.
	if (!TargetActor)
	{
		WepInfo.TargetActor = nullptr;
		return WepInfo;
	}

	//Clear Target and Return if the Target is about to be Destroyed.
	if (TargetActor->IsPendingKillPending())
	{
		ClearTarget();
		WepInfo.TargetActor = nullptr;
		return WepInfo;
	}

	//Add out Target actor to the Weapon info struct and return it.
	WepInfo.TargetActor = TargetActor;
	return WepInfo;
}

void UWeaponManager::SetTarget(AActor* NewTarget)
{
	TargetActor = NewTarget;
}

AActor* UWeaponManager::GetTarget() const
{
	return TargetActor;
}

void UWeaponManager::ClearTarget()
{
	TargetActor = nullptr;
}

