// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Create a Box Collision.
	ProCol = CreateDefaultSubobject<UBoxComponent>(TEXT("ProCol"));
	//Use our Box Collision as Main root to avoid using the more Heavy Static mesh for Hit results.
	RootComponent = ProCol;

	//Create our Projectile Static mesh Model.
	ProMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProMesh"));
	ProMesh->SetupAttachment(ProCol);

	//Add a Projectile Movement component.
	ProMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));

	//Set Default Values
	
	Damage = 10.0f;
	ProMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProMesh->SetGenerateOverlapEvents(false);
	ProCol->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle DestroyHandle;
	GetWorld()->GetTimerManager().SetTimer(DestroyHandle, this, &ABaseProjectile::DestroyProjectile, 3.0f, true);

	FTimerHandle ColTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(ColTimerHandle, this, &ABaseProjectile::ActivateCollision, GetWorld()->GetDeltaSeconds(), true);
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Custom Destroy Function in case we want to trigger some effect before the actual Destruction of the Actor.
void ABaseProjectile::DestroyProjectile()
{
	//Destroy the Actor.
	Destroy();	
}

void ABaseProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	//Spawn a Particle when we hit something.
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, HitLocation, HitNormal.Rotation(), FVector(2.0f));
	FDamageEvent DmgEvent;
	Other->TakeDamage(Damage, DmgEvent, this->GetInstigatorController(), GetOwner());

	//Apply Damage to the Hit actor.
	UGameplayStatics::ApplyDamage(Other, Damage, this->GetInstigatorController(), GetOwner(), ProDamageType);
	//Call out Custom destroy function.
	DestroyProjectile();
}

void ABaseProjectile::ActivateCollision()
{
	ProCol->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}