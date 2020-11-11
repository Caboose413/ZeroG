// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseShip.h"

// Sets default values
ABaseShip::ABaseShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create our Static mesh Ship model.
	ShipModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ship"), false);
	//Set the Ship model to our Root Component.
	RootComponent = ShipModel;

	

	//Set Default Values
	ShipModel->SetSimulatePhysics(true);
	
	ThrustStrength = 250.0f;
	RotationStrength = 25.0f;

	BackWardStrength = 0.3f;
	SurgeStrength = 1.0f;
	SwayStrength = 0.3f;
	HeaveStrength = 0.3f;
	
}

// Called when the game starts or when spawned
void ABaseShip::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Setup Mouse and Keyboard Input events.
	PlayerInputComponent->BindAxis("Yaw", this, &ABaseShip::Yaw);
	PlayerInputComponent->BindAxis("Pitch", this, &ABaseShip::Pitch);
	PlayerInputComponent->BindAxis("Roll", this, &ABaseShip::Roll);

	PlayerInputComponent->BindAxis("Surge", this, &ABaseShip::Surge);
	PlayerInputComponent->BindAxis("Sway", this, &ABaseShip::Sway);
	PlayerInputComponent->BindAxis("Heave", this, &ABaseShip::Heave);
}


//Main function to Apply Thrust to our Ship.
void ABaseShip::AddThrust(const FVector Direction, const float InputStrength) const
{
	const FVector ThrustDirection = Direction.GetSafeNormal() * ThrustStrength * InputStrength;
	ShipModel->AddForce(ThrustDirection, NAME_None, true);
}

//Main funciton to apply a physics rotation to our ship.
void ABaseShip::AddRotation(const FVector Axis) const
{
	ShipModel->AddTorqueInDegrees(Axis.GetClampedToMaxSize(1.0f) * RotationStrength, NAME_None, true);
}

//Input
void ABaseShip::Yaw(const float Input)
{
	//Create a Vector which rotates around our Up/Yaw Rotation.
	const FVector ShipUp = ShipModel->GetUpVector();
	AddRotation(ShipUp * Input);
}

void ABaseShip::Pitch(const float Input)
{
	//Create a Vector which rotates around our Right/Pitch Rotation.
	const FVector ShipRight = ShipModel->GetRightVector();
	AddRotation(ShipRight * Input);
}

void ABaseShip::Roll(const float Input)
{
	//Create a Vector which rotates around our Forward/Roll Rotation.
	const FVector ShipForward = ShipModel->GetForwardVector();
	AddRotation(ShipForward * Input);
}

//Apply thrust in the Forward/Backward Direction.
void ABaseShip::Surge(const float Input)
{
	//Create a Vector which points Forward.
	const FVector ShipForward = ShipModel->GetForwardVector();
	AddThrust(ShipForward * Input, SurgeStrength * Input > 0 ? 1.0f : BackWardStrength);
}

//Apply thrust in the Right/Left Direction.
void ABaseShip::Sway(const float Input)
{
	//Create a Vector which points Right.
	const FVector ShipRight = ShipModel->GetRightVector();
	AddThrust(ShipRight * Input, SwayStrength);
}

//Apply thrust in the Up/Down Direction.
void ABaseShip::Heave(const float Input)
{
	//Create a Vector which points Up.
	const FVector ShipUp = ShipModel->GetUpVector();
	AddThrust(ShipUp * Input, HeaveStrength);
}

