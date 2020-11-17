// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseShip.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

// Sets default values
ABaseShip::ABaseShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create our Static mesh Ship model.
	ShipModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ship"), false);
	//Set the Ship model to our Root Component.
	RootComponent = ShipModel;

	WeaponManager = CreateDefaultSubobject<UWeaponManager>(TEXT("WeaponManager"));
	HealthManager = CreateDefaultSubobject<UHealthManager>(TEXT("HealthManager"));


	
	//Set Default Values
	ShipModel->SetSimulatePhysics(true);
	
	ThrustStrength = 250.0f;
	RotationStrength = 25.0f;

	BackWardStrength = 0.3f;
	SurgeStrength = 1.0f;
	SwayStrength = 0.3f;
	HeaveStrength = 0.3f;

	ScanArea = 10.0f;

	LastRandomTarget = -1;
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

	//Setup Keyboard Input events.
	PlayerInputComponent->BindAction("CycleTarget", IE_Pressed, this, &ABaseShip::CycleTarget);
	
	//Setup Mouse Axis events.
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

void ABaseShip::CycleTarget()
{
	TArray<AActor*> TargetsInCone;
	const FVector OwnerForward = GetActorForwardVector();
	const FVector OwnerLocation = GetActorLocation();
	
	for (TActorIterator<ABaseShip> Ship(GetWorld()); Ship; ++Ship)
	{
		if (*Ship == this || Ship->IsPendingKillPending())
		{
			continue;
		}
		//UE_LOG(LogTemp, Warning, TEXT("Ship %s"), *Ship->GetName());
		const FVector TargetNormal = (Ship->GetActorLocation() - OwnerLocation).GetSafeNormal();
		const float TargetDot = FVector::DotProduct(OwnerForward, TargetNormal);
		
		const float DotToDegree = FMath::Lerp(180.0f, 0.0f, TargetDot);
		
		//UE_LOG(LogTemp, Warning, TEXT("Dot %f Deg %f"),TargetDot, DotToDegree);
		if (DotToDegree < ScanArea)
		{
			TargetsInCone.Add(*Ship);
		}
	}

	TargetsInCone.Remove(WeaponManager->GetTarget());
	
	const int RandomTarget = FMath::RandHelper(TargetsInCone.Num());

	if (TargetsInCone.Num() > 0)
	{
		WeaponManager->SetTarget(TargetsInCone[RandomTarget]);
		LastRandomTarget = RandomTarget;
	}
	else
	{
		WeaponManager->ClearTarget();
	}

	TargetsInCone.Empty();
}

void ABaseShip::OnSendDeath()
{
	DestroyShip();
	//UE_LOG(LogTemp, Warning, TEXT("d1"));
}

void ABaseShip::DestroyShip()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorLocation(), FRotator(), FVector(25.0f));
	Destroy();
}



