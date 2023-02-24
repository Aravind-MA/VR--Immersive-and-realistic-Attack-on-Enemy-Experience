// Fill out your copyright notice in the Description page of Project Settings.


#include "SecondaryPlayer.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Sound/SoundCue.h"
#include "Engine/World.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MotionControllerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h" 
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/EngineTypes.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"
#include "GameFramework/Pawn.h"
#include "MyEnemyClass.h"
#include "Bullet_Hit_Interface.h"
#include "EnemyActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyAsistanceController.h"

// Sets default values
ASecondaryPlayer::ASecondaryPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sec_VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Sec_VRRoot"));
	Sec_VRRoot->SetupAttachment(GetRootComponent());



	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(Sec_VRRoot);

	Rocketbody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Rocketbody"));
	Rocketbody->SetupAttachment(Camera);

	GunPossesion = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunPossesion"));
	GunPossesion->SetupAttachment(Rocketbody);

	RightMC = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightMC"));
	RightMC->SetupAttachment(Sec_VRRoot);
	RightMC->SetTrackingSource(EControllerHand::Right);


	LeftMC = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftMC"));
	LeftMC->SetupAttachment(Sec_VRRoot);
	LeftMC->SetTrackingSource(EControllerHand::Left);

}



// Called when the game starts or when spawned
void ASecondaryPlayer::BeginPlay()
{
	Super::BeginPlay();

	//collision respone 
	Rocketbody->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);


}

// Called every frame
void ASecondaryPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASecondaryPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &ASecondaryPlayer::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Right"), this, &ASecondaryPlayer::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("snapYawRotation"), this, &ASecondaryPlayer::Rotation);
	PlayerInputComponent->BindAction(TEXT("PossessRocket"), IE_Pressed, this, &ASecondaryPlayer::possess);//Spawn Function Input
	PlayerInputComponent->BindAction(TEXT("TriggerRight"), IE_Pressed, this, &ASecondaryPlayer::Fire);//"Possession" spawn Function Input

}
void ASecondaryPlayer::MoveForward(float Value)
{
	AddMovementInput(Value * Camera->GetForwardVector());
}

void ASecondaryPlayer::MoveRight(float Value)
{
	AddMovementInput(Value * Camera->GetRightVector());
}
void ASecondaryPlayer::Rotation(float Local)
{

	AddControllerYawInput(Local * RotationRate * GetWorld()->GetDeltaSeconds());

}
void ASecondaryPlayer::Possession_Implementation()//Possession Interface fumction implementation
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(this);
}

void ASecondaryPlayer::Fire()//Spawn Function
{


	if (MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, GunPossesion->GetSocketLocation(FName("BarrelSocket")));
	}


	FVector Start = GunPossesion->GetSocketLocation(FName("BarrelSocket"));
	FVector End = Start + GunPossesion->GetForwardVector() * MaxRange;
	//From "gun Spawn socket" to "bullet Hit"
	bool bSucesss = GetWorld()->LineTraceSingleByChannel(HitFire, Start, End, ECollisionChannel::ECC_Visibility);
	UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, GetActorLocation());

	if (bSucesss)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticleEffect, HitFire.Location);
	}

	AEnemyActor* HitEnemy = Cast<AEnemyActor>(HitFire.GetActor());

	if (HitEnemy)
	{

		if (HitFire.BoneName == HitEnemy->GetHeadSocket())//headsocket attack 
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitHeadSocket_FX, HitFire.Location);

			UGameplayStatics::ApplyDamage(HitFire.GetActor(), HeadshotDamage, GetController(), this, UDamageType::StaticClass());
		}


	}

	//Execute BulletHit interface function
	if (HitFire.Actor.IsValid())
	{
		if (HitFire.Actor->GetClass()->ImplementsInterface(UBullet_Hit_Interface::StaticClass()))
		{
			IBullet_Hit_Interface::Execute_BulletHit(HitFire.GetActor(), HitFire);

			

			if (HitFire.Actor == HitEnemy)
			{
				UGameplayStatics::ApplyDamage(HitFire.GetActor(), Damage, GetController(), this, UDamageType::StaticClass());
			}


		}
	}
}

void ASecondaryPlayer::possess()//"Possession" spawn Function
{

	FVector FromStart = GunPossesion->GetSocketLocation(FName("muzzleflashSocket"));
	FVector ToEnd = FromStart + GunPossesion->GetForwardVector() * PossessMaxRange;
	bool bPossess = GetWorld()->LineTraceSingleByChannel(HitPossess, FromStart, ToEnd, ECollisionChannel::ECC_Visibility);
	UGameplayStatics::PlaySoundAtLocation(this, PossessSound, GetActorLocation());

	if (bPossess)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PossessHit_FX, HitPossess.Location);
	}

	//Execute posession interface function
	if (HitPossess.Actor.IsValid())
	{
		if (HitPossess.Actor->GetClass()->ImplementsInterface(UBullet_Hit_Interface::StaticClass()))
		{
			IBullet_Hit_Interface::Execute_Possession(HitPossess.GetActor());

		}
	}

	
}


