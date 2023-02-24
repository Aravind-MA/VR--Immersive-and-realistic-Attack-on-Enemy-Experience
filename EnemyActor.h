// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnergyWeapon.h"
#include "Bullet_Hit_Interface.h"

#include "EnemyActor.generated.h"

UCLASS()														//Interface
class VR_AI_PROGRAMING_API AEnemyActor : public ACharacter, public IBullet_Hit_Interface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyActor();

	//HitReact Anim function
	void AnimMontage(FName SectionName, float Rate);

	//Attack Anim function , BlueprintCallable
	UFUNCTION(BlueprintCallable)
		void AttackAnim(FName Section, float PlayRate);

	// it is a pure blueprint //Attack Anim Section name
	UFUNCTION(BlueprintPure)
		FName SectionName();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	//Widget HealthBar

	//BlueprintNativeEvent is also defined in C++, but that can be overridden in Blueprint
	UFUNCTION(BlueprintNativeEvent)
	void ShowHealthBar();
	void ShowHealthBar_Implementation();

	//blueprint impletable event can be overridden in Blueprint and Do not provide a body for this function  
	UFUNCTION(BlueprintImplementableEvent)
		void HideHealthBar();

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//BulletHit_Implementation Function(Interface)
	virtual void BulletHit_Implementation(FHitResult Hit) override;

	//Health
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;

	void Die();


	//SniffingSphere begin and end

	UFUNCTION()
	void SniffingSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void SniffingSphereEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	//AttackSpher begin and end

	UFUNCTION()
	void AttackSphereOverlapBegin(UPrimitiveComponent*OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void AttackSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
	

	

	

private:

	//AttackedSocket

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackedSocket", meta = (AllowPrivateAccess = "true"))
		FName HeadSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackedSocket", meta = (AllowPrivateAccess = "true"))
		FName LeftSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackedSocket", meta = (AllowPrivateAccess = "true"))
		FName RightSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackedSocket", meta = (AllowPrivateAccess = "true"))
		FName BackSocket;


	//Sniffing

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sniffing", meta = (AllowPrivateAccess = "true"))
		float SniffingRadius = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sniffing", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SniffingSphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sniffing", meta = (AllowPrivateAccess = "true"))
		class USoundCue* SniffingSound;


	//AttackSphere

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackSphere", meta = (AllowPrivateAccess = "true"))
		class USphereComponent* AttackSphere;


	//Equipment

	UPROPERTY()
	class AEnergyWeapon* RightHandWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment", meta = (AllowPrivateAccess = "true"))
	FName Socketequ;
	
	

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnergyWeapon> EnemyWeaponInHand;



	//Combat
	//particle FX  when hit by bullets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		class UParticleSystem* ImpactParticle;

	//Sound to play when hit by bullets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		class USoundCue* bulletImpactSound;

	//Sound to play when Attack on bone sockets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		class USoundCue* ImpactOnAttackedBoneSocket;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		class USoundCue* DeathShotSound;

	// Enemy screamming sound when he got a bullet shot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		class USoundCue* Enemyscreamming;


	//Health

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
		float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
		FTimerHandle HealthBarTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
		float HealthBarTimeDisplay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
		float BaseDamage;



	//AnimMontage

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage", meta = (AllowPrivateAccess = "true"))
		class UAnimationAsset* Death;


	//Behavior Tree

	/** Behavior tree for the AI Character */
	UPROPERTY(EditAnywhere, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree* BehaviorTree;

	/** Point for the enemy to move to */												//move it around in the viewport
	UPROPERTY(EditAnywhere, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
		FVector PatrolPoint;
																							//move it around in the viewport
	UPROPERTY(EditAnywhere, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
		FVector PatrolPoint2;

	UPROPERTY(EditAnywhere, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
	class AEnemy_AIController* EnemyController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
		bool IsAttacking;


	FName RoarUp;
	FName FastL;
	FName FastR;
	FName Knock;
	FName Four;
	
	

public:
		
	FORCEINLINE FName GetHeadSocket() const { return HeadSocket; }
	FORCEINLINE FName GetBackSocket() const { return BackSocket; }
	FORCEINLINE FName GetLeftSocket() const { return LeftSocket; }
	FORCEINLINE FName GetRightSocket() const { return RightSocket;}
	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

};
