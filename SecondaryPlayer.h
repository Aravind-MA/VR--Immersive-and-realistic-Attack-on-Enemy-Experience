// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MotionControllerComponent.h"
#include "Bullet_Hit_Interface.h"

#include "SecondaryPlayer.generated.h"

UCLASS()															//Interface
class VR_AI_PROGRAMING_API ASecondaryPlayer : public ACharacter, public IBullet_Hit_Interface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASecondaryPlayer();

	//Character Movemnt
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Rotation(float Local);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Possession_Implementation Function(Interface)
	virtual void Possession_Implementation() override;

	//Shooting Combat
	void Fire();

	//Possesion Function
	void possess();

private:

	//Components
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Sec_VRRoot;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "StaticMeshComponent", meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Rocketbody;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MotionControllerComponent", meta = (AllowPrivateAccess = "true"))
		class UMotionControllerComponent* RightMC;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "StaticMeshComponent", meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* GunPossesion;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MotionControllerComponent", meta = (AllowPrivateAccess = "true"))
		class UMotionControllerComponent* LeftMC;



	
	//Shooting combat

	FHitResult HitFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		class USoundCue* SpawnSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		float MaxRange = 10000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		class UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		class UParticleSystem* HitParticleEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		class UParticleSystem* HitHeadSocket_FX;


	


	// Possession

	FHitResult HitPossess;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		float PossessMaxRange = 10000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		class USoundCue* PossessSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		class UParticleSystem* PossessHit_FX;


	//Enemy's health aspects

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyHealth", meta = (AllowPrivateAccess = "true"))
		float Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyHealth", meta = (AllowPrivateAccess = "true"))
		float HeadshotDamage = 25.f;




	//Movement Rotation
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float RotationRate = 0.0f;







};
