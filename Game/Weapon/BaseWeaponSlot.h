// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//Engine
#include "CoreMinimal.h"
#include "Components/ChildActorComponent.h"

//ZeroG
#include "BaseWeaponSlot.generated.h"

/**
 * Class to dynamically Switch out Weapons on a Slot.
 */

UENUM(BlueprintType)
enum ESlotType
{
	Static      UMETA(DisplayName = "Static"),
    Gimbal   	UMETA(DisplayName = "Gimbal"),
    Auto        UMETA(DisplayName = "Auto")
};    

UCLASS(Blueprintable)
class ZEROG_API UBaseWeaponSlot : public UChildActorComponent
{
	GENERATED_BODY()

public:
    UBaseWeaponSlot();


	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	


    UFUNCTION()
    void ShootSlot(bool Shooting);

    UPROPERTY(EditAnywhere)
    FString Group;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ESlotType>  GimbalType;

	UPROPERTY(EditAnywhere)
	float MaxGimbal;
	
};
