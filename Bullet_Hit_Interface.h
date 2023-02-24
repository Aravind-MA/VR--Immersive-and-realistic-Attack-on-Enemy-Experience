// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Bullet_Hit_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBullet_Hit_Interface : public UInterface 
{
	GENERATED_BODY()
};


/**
 * 
 */
class VR_AI_PROGRAMING_API IBullet_Hit_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//BlueprintNativeEvent can be declared in C++ that can have a default behavior and it can be implemented in both C++/BP //it should be overridden  while implementing on another class inheritence
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BulletHit(FHitResult Hit); // BulletHit Interfaces

	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	//void HitScaling();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Possession(); //Possession interfaces
};
