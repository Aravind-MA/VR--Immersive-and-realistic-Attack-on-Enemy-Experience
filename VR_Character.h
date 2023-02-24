// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MotionControllerComponent.h"
#include "Bullet_Hit_Interface.h"

#include "VR_Character.generated.h"


UCLASS()														//Interface
class VR_AI_PROGRAMING_API AVR_Character : public ACharacter, public IBullet_Hit_Interface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVR_Character();

	//Character Movemnt
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Rotation(float Local);

	//Teleportation
	bool FindTeleportDestination(FVector& OutLocation);
	void DestinationMarkLocation();
	void TeleportFinish();

	//Shooting Combat
	void weapon();

	//Possesion Function
	void Possess();

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

private:

	//Components
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* VRRoot;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "body", meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* body;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* DestinationMark;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MotionController", meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* RightMotionController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkeletalMeshComponent", meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* RightHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkeletalMeshComponent", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Melle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MotionController", meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* LeftMotionController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkeletalMeshComponent", meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* LeftHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkeletalMeshComponent", meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Gun;







	// Teleportation
	float TeleportProjectileRadius = 10.f;

	UPROPERTY(EditAnywhere)
		float TeleportProjectileSpeed = 800.f;

	UPROPERTY(EditAnywhere)
		float TeleportSimulationTime = 2.f;
	
	UPROPERTY(EditAnywhere)
		FVector TeleportationExtent = FVector(100.f, 100.f, 100.f);





	//Possession
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Possession", meta = (AllowPrivateAccess = "true"))
		float PossessMaxRange = 10000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Possession", meta = (AllowPrivateAccess = "true"))
		class USoundCue* PossessSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Possession", meta = (AllowPrivateAccess = "true"))
		class UParticleSystem* PossessHit_FX;

	FHitResult HitPossess;



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






	//Movement Rotation
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float RotationRate = 0.0f;


	
	



	//Enemy's health aspects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyHealth", meta = (AllowPrivateAccess = "true"))
		float Damage = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyHealth", meta = (AllowPrivateAccess = "true"))
		float HeadshotDamage=25.f;

	

	
	

	





	


	
	
};
