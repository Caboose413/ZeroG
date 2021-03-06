// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

#include "Kismet/GameplayStatics.h"
#include "ZeroG/Pawn/BaseShip.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RPM = 1500.0f;


	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = RootScene;
	
	//Create our Weapon Static mesh model.
	WepMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProMesh"));
	WepMesh->SetupAttachment(RootScene);

	//Create a Helper for getting the actual spawn location.
	SpawnLoc = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnLoc"));
	SpawnLoc->SetupAttachment(RootScene);

	ProSpeed = 85000.0f;

	WeaponSpread = 10.0f;
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseWeapon::ShootWeapon(bool Shooting)
{
	if (Shooting)
	{
		//Calculate the delay time for the Current RPM.
		const float ShootDelay = 60.0f / RPM;

		//Start a Timer to execute the shooting function.
		GetWorld()->GetTimerManager().SetTimer(ShootTimer, this, &ABaseWeapon::ExeShoot, ShootDelay, true);

		//Already shoot a bullet on upon pressing fire button.
		ExeShoot();
	}
	else
	{
		//Stop timer when we let go of the fire button.
		GetWorld()->GetTimerManager().ClearTimer(ShootTimer);
	}
}

//Loop this function with a timer to create repeating shoots.
void ABaseWeapon::ExeShoot()
{
	ABaseShip* OwningShip = Cast<ABaseShip>(GetOwner());
	if (OwningShip)
	{
		OwningShip->WeaponManager->RefreshWeaponData();
	}

	//Get the Owner Velocity.
	const FVector OwnerVel = OwningShip->GetVelocity();

	//Get the Weapons Transform.
	FTransform WepTrans = SpawnLoc->GetComponentTransform();

	//Offset the Spawn Location with the Velocity so the Projectile spawns in the correct location.
	WepTrans.SetLocation(WepTrans.GetLocation() - OwnerVel / 100.0f);

	//Spawn Projectile Deferred so we can set some default values.
	AActor* Pro = (UGameplayStatics::BeginDeferredActorSpawnFromClass(this, Projectile, WepTrans));
	if (!Pro)
	{
		return;
	}
	
	//Make a Random rotation to add Weapon Spread.
	FVector RandomRot;
	RandomRot.X = FMath::FRandRange(-WeaponSpread, WeaponSpread);
	RandomRot.Y = FMath::FRandRange(-WeaponSpread, WeaponSpread);
	RandomRot.Z = FMath::FRandRange(-WeaponSpread, WeaponSpread);

	//Pre cache the actors forward vector.
	FVector Forward = GetActorForwardVector();
	
	//Add the Random Rotation to our Forward Vector.
	Forward += (RandomRot * 0.001f);
	
	//Apply owner Velocity to the Projectile so the Spawned projectile matches the ships speed.
	const FVector AdjustedVelocity = OwnerVel + (Forward * ProSpeed);

	//Spawn the Actual Projectile;
	UGameplayStatics::FinishSpawningActor(Pro, WepTrans);
	
	//Apply the Adjusted Velocity.
    Cast<ABaseProjectile>(Pro)->ProMovement->Velocity = AdjustedVelocity;
}

//Function to set the Owner/OwningShip of this weapon.
void ABaseWeapon::SetOwningShip(AActor* NewOwner)
{
	if (NewOwner)
	{
		SetOwner(NewOwner);
	}
}

