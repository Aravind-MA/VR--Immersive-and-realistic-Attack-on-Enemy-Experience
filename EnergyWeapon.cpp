// Fill out your copyright notice in the Description page of Project Settings.


#include "EnergyWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AEnergyWeapon::AEnergyWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AEnergyWeapon::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AEnergyWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnergyWeapon::BulletHit_Implementation(FHitResult HitResult)
{
	if (SoundEffect)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SoundEffect, GetActorLocation());//particle to spawn while shoted
	}
	if (ParticleFX_on_weapon)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleFX_on_weapon, HitResult.Location, FRotator(0.f), true);//sound to play while firing
	}
	
}

