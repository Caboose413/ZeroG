// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthManager.h"

// Sets default values for this component's properties
UHealthManager::UHealthManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	MaxHealth = 100.0f;
	CurHealth = 100.0f;
	CanTakeDmg = true;
	// ...
}


// Called when the game starts
void UHealthManager::BeginPlay()
{
	Super::BeginPlay();
	// ...
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthManager::TakeDmg);
}


// Called every frame
void UHealthManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthManager::SetHealth(const float ToSet)
{
	CurHealth = FMath::Clamp(ToSet, 0.0f, MaxHealth);
	//UE_LOG(LogTemp, Warning, TEXT("Health %f"), GetHealth());
	if (GetHealth() <= 0.0f)
	{
		Die();
	}
}

void UHealthManager::AddHealth(const float ToAdd)
{
	//UE_LOG(LogTemp, Warning, TEXT("HealthManager %f"), ToAdd);
	SetHealth(GetHealth() + ToAdd);
}

float UHealthManager::GetHealth() const
{
	return CurHealth;
}

void UHealthManager::TakeDmg(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (CanTakeDmg)
	{
		AddHealth(-Damage);
	}
}

void UHealthManager::Die()
{
    IZeroGInterface* DeathInterface = Cast<IZeroGInterface>(GetOwner());
    if (DeathInterface)
    {
	    DeathInterface->OnSendDeath();
    }
}

