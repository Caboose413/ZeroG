// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ZeroGInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UZeroGInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZEROG_API IZeroGInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION()
	virtual void OnSendDeath();
	
};
