// Fill out your copyright notice in the Description page of Project Settings.


#include "ZeroGFunctions.h"

FVector UZeroGFunctions::GetAimPrediction(AActor* Owner, AActor* Target, float Velocity)
{
    const float PredictionDistance = FVector::Dist(Target->GetActorLocation(), Owner->GetActorLocation()) / Velocity;

    const FVector VelocityDifference = Target->GetVelocity() - Owner->GetVelocity();

    const FVector PredictedLocation = (VelocityDifference * PredictionDistance) + Target->GetActorLocation();

    return PredictedLocation;
}