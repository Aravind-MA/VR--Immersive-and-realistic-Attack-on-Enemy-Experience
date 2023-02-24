// Fill out your copyright notice in the Description page of Project Settings.


#include "VR_Character.h"
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
#include "MyPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystem.h"
// Sets default values
AVR_Character::AVR_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	VRRoot->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VRRoot);

	Player = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Player"));
	Player->SetupAttachment(Camera);

	body = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("body"));
	body->SetupAttachment(Camera);

	RightMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightMotionController"));
	RightMotionController->SetupAttachment(VRRoot);
	RightMotionController->SetTrackingSource(EControllerHand::Right);

	RightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHand"));
	RightHand->SetupAttachment(RightMotionController);

	Melle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Melle"));
	Melle->SetupAttachment(RightHand);

	DestinationMark = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DestinationMark"));
	DestinationMark->SetupAttachment(VRRoot);

	LeftMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftMotionController"));
	LeftMotionController->SetupAttachment(VRRoot);
	LeftMotionController->SetTrackingSource(EControllerHand::Left);

	LeftHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHand"));
	LeftHand->SetupAttachment(LeftMotionController);

	Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	Gun->SetupAttachment(LeftHand);

}


// Called when the game starts or when spawned
void AVR_Character::BeginPlay()
{
	Super::BeginPlay();
	
	

	
	
}


// Called every frame
void AVR_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DestinationMarkLocation();
}

//TeleportDestination 
bool AVR_Character::FindTeleportDestination(FVector& OutLocation)
{
	//this method is similar to LineTraceSingleByChannel()

	FVector Look = RightMotionController->GetForwardVector();

	FVector Location = RightMotionController->GetComponentLocation() + Look * 15.0f;

	FPredictProjectilePathParams Params(TeleportProjectileRadius, Location, Look * TeleportProjectileSpeed, TeleportSimulationTime, ECollisionChannel::ECC_MAX, this);
	Params.DrawDebugType = EDrawDebugTrace::ForOneFrame;
	Params.bTraceComplex = true;

	FPredictProjectilePathResult Result;

	bool bSucess = UGameplayStatics::PredictProjectilePath(this, Params, Result);
	
	if (!bSucess) return false;
	
//Teleportation to detect NAV mesh bounds volume location
	FNavLocation Navigation;
											//GetCurrent(GetWorld())->This World represents a map(Level)
	bool bNavigative = UNavigationSystemV1::GetCurrent(GetWorld())->ProjectPointToNavigation(Result.HitResult.Location, Navigation, TeleportationExtent);
	if (!bNavigative)return false;
	OutLocation = Navigation.Location;
	return true;

}
void AVR_Character::DestinationMarkLocation()
{

//To place the DestinationMark on the "NAV mesh bounds volume"
	FVector Location;
	bool bHasDestination = FindTeleportDestination(Location);
	if (bHasDestination)
	{

		DestinationMark->SetVisibility(true);
		
		DestinationMark->SetWorldLocation(Location);

	}
	else
	{
		DestinationMark->SetVisibility(false);
	}
}

void AVR_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &AVR_Character::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Right"), this, &AVR_Character::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("snapYawRotation"), this, &AVR_Character::Rotation);
	PlayerInputComponent->BindAction(TEXT("Teleportation"), IE_Released, this, &AVR_Character::TeleportFinish);
	PlayerInputComponent->BindAction(TEXT("Triggerleft"), IE_Pressed, this, &AVR_Character::weapon);//Spawn Function Input
	PlayerInputComponent->BindAction(TEXT("PossessFun"), IE_Pressed, this, &AVR_Character::Possess);//"Possession" spawn Function Input
}



void AVR_Character::MoveForward(float Value)
{
	AddMovementInput(Value * Camera->GetForwardVector());
}

void AVR_Character::MoveRight(float Value)
{
	AddMovementInput(Value * Camera->GetRightVector());
}
void AVR_Character::Rotation(float Local)
{

	AddControllerYawInput(Local * RotationRate * GetWorld()->GetDeltaSeconds());

}
void AVR_Character::Possession_Implementation()//Possession Interface fumction implementation
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(this);
}


void AVR_Character::weapon()//Spawn Function
{


	if (MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, Gun->GetSocketLocation(FName("BarrelSocket")));
	}


	FVector Start = Gun->GetSocketLocation(FName("BarrelSocket"));
	FVector End = Start + Gun->GetForwardVector() * MaxRange;
	//From guns Spawn socket to bullet Hit
	bool bSucesss = GetWorld()->LineTraceSingleByChannel(HitFire, Start, End, ECollisionChannel::ECC_Visibility);
	UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, HitFire.Location);

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
	
	
	//We can practice both ways to Execute the interface whether by cast or by ImplementsInterface()




	/*if (HitFire.Actor.IsValid())
	{
		IBullet_Hit_Interface* BulletHit = Cast<IBullet_Hit_Interface>(HitFire.GetActor());

		if (BulletHit)
		{
				BulletHit->BulletHit_Implementation(HitFire);
				UGameplayStatics::ApplyDamage(HitFire.GetActor(), Damage, GetController(), this, UDamageType::StaticClass());
		}
	}*/
	


												//(OR)


	//Execute BulletHit interface function
	if (HitFire.Actor.IsValid())
	{
		if (HitFire.Actor->GetClass()->ImplementsInterface(UBullet_Hit_Interface::StaticClass()))
		{
			IBullet_Hit_Interface::Execute_BulletHit(HitFire.GetActor(), HitFire);

			if (HitFire.Actor == HitEnemy)//only if it hits the EnemyActor
			{
				UGameplayStatics::ApplyDamage(HitFire.GetActor(), Damage, GetController(), this, UDamageType::StaticClass());
			}

			
		}
	}
	
	/*if (HitFire.Actor.IsValid())
	{
		if (HitFire.Actor->GetClass()->ImplementsInterface(UBullet_Hit_Interface::StaticClass()))
		{
			IBullet_Hit_Interface::Execute_HitScaling(HitFire.GetActor());

		}
	}*/

	
}


void AVR_Character::Possess()//"Possession" spawn Function
{
	FVector FromStart = Gun->GetSocketLocation(FName("Posssess"));
	FVector ToEnd = FromStart + Gun->GetForwardVector() * PossessMaxRange;
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


void AVR_Character::TeleportFinish()
{
	FVector Destination = DestinationMark->GetComponentLocation();
	SetActorLocation(Destination);

}