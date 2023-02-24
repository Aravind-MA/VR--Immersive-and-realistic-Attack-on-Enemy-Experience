// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet_Hit_Interface.h"

#include "EnergyWeapon.generated.h"

UCLASS()
class VR_AI_PROGRAMING_API AEnergyWeapon : public AActor, public IBullet_Hit_Interface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnergyWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BulletHit_Implementation(FHitResult HitResult) override;

private:
	//particle to spawn while shoted
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		class UParticleSystem* ParticleFX_on_weapon;
	//sound to play while firing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		class USoundCue* SoundEffect;
};
