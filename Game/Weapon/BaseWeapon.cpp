// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RPM = 1500.0f;

	//Create our Weapon Static mesh model.
	WepMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProMesh"));
	RootComponent = WepMesh;

	//Create a Helper for getting the actual spawn location.
	SpawnLoc = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnLoc"));
	SpawnLoc->SetupAttachment(WepMesh);
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
		float ShootDelay = 60.0f / RPM;

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
	//Get the Owner Velocity.
	const FVector OwnerVel = GetOwner()->GetVelocity();

	//Get the Weapons Transform.
	FTransform WepTrans = SpawnLoc->GetComponentTransform();

	//Offset the Spawn Location with the Velocity so the Projectile spawns in the correct location.
	WepTrans.SetLocation(WepTrans.GetLocation() - OwnerVel / 100.0f);

	//Spawn and create a Reference to the Projectile.
	AActor* Pro = GetWorld()->SpawnActor<ABaseProjectile>(Projectile, WepTrans);

	//Spawn Projectile Deferred so we can set the collision to Ignore the Owner and void Hitting ourself.
	//AActor* Pro = (UGameplayStatics::BeginDeferredActorSpawnFromClass(this, Projectile, WepTrans));
	//Cast<ABaseProjectile>(Pro)->ProCol->MoveIgnoreActors.Add(GetOwner());
	//UGameplayStatics::FinishSpawningActor(Pro, WepTrans);
	
	//Get the Initial Speed of our Projectile.
	const float InitSpeed = Cast<ABaseProjectile>(Pro)->ProMovement->InitialSpeed;

	//Apply owner Velocity to the Projectile so the Spawned projectile matches the ships speed.
	const FVector AdjustedVelocity = OwnerVel + (GetActorForwardVector() * InitSpeed);

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

