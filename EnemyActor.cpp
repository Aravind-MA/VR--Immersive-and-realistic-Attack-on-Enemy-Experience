// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyActor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h" 
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Animation/AnimInstance.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Enemy_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "VR_Character.h"
#include "GameFramework/Controller.h"
#include "NavigationSystem.h"
#include "EnemytAnimInstance.h"
#include "Components/BoxComponent.h"

// Sets default values
AEnemyActor::AEnemyActor():
		Health(100.f),
		MaxHealth(100.f),
		RoarUp(TEXT("Roar")),
		FastL(TEXT("FastLAttack")),
		FastR(TEXT("RA_Attack")),
		Knock(TEXT("KnockUp")),
		Four(TEXT("FourStrikes")),
		BaseDamage(20.f)

{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//Sniffing

	SniffingSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SniffingSphereComponent"));
	SniffingSphereComponent->SetupAttachment(GetRootComponent());
	

	//AttackSphere

	AttackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"));
	AttackSphere->SetupAttachment(GetRootComponent());

	
}

// Called when the game starts or when spawned
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();
	
	//SniffingSphere begin and end

	SniffingSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::SniffingSphere);

	SniffingSphereComponent->OnComponentEndOverlap.AddDynamic(this, &AEnemyActor::SniffingSphereEnd);


	//AttackSpher begin and end

	AttackSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::AttackSphereOverlapBegin);

	AttackSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyActor::AttackSphereOverlapEnd);

	//SocketName

	HeadSocket = GetMesh()->GetSocketBoneName(TEXT("HeadSocket"));

	LeftSocket = GetMesh()->GetSocketBoneName(TEXT("LeftSocket"));

	RightSocket = GetMesh()->GetSocketBoneName(TEXT("RightSocket"));

	BackSocket = GetMesh()->GetSocketBoneName(TEXT("BackSocket"));


	//collision respone 
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);


	//Equip
	Socketequ = (TEXT("weaponRSocket"));
	FTransform socketTransform = GetMesh()->GetSocketTransform(Socketequ, ERelativeTransformSpace::RTS_World);

	RightHandWeapon = GetWorld()->SpawnActor<AEnergyWeapon>(EnemyWeaponInHand, socketTransform);

	if (RightHandWeapon)
	{
		RightHandWeapon->K2_AttachRootComponentTo(GetMesh(), Socketequ, EAttachLocation::SnapToTarget, true);//Attach 
	}


	//PatrolPoints helps the Enemy to move from one patrol point to another patrol point

	EnemyController = Cast<AEnemy_AIController>(GetController());

	FVector WorldPatrolPoint = UKismetMathLibrary::TransformLocation(
		GetActorTransform(),
		PatrolPoint);
	DrawDebugSphere(
		GetWorld(),
		WorldPatrolPoint,
		25.f,
		12,
		FColor::Red,
		true);

	FVector WorldPatrolPoint2 = UKismetMathLibrary::TransformLocation(
		GetActorTransform(),
		PatrolPoint2);
	DrawDebugSphere(
		GetWorld(),
		WorldPatrolPoint2,
		25.f,
		12,
		FColor::Orange,
		true);
	
	
	//BehaviorTree access through GetBlackboardComponent
		if (EnemyController)
		{
			EnemyController->GetBlackboardComponent()->SetValueAsVector(
				TEXT("PatrolPoint"),
				WorldPatrolPoint);

			EnemyController->GetBlackboardComponent()->SetValueAsVector(
				TEXT("PatrolPoint2"),
				WorldPatrolPoint2);

			EnemyController->RunBehaviorTree(BehaviorTree);
		}

		
}


void AEnemyActor::AttackSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;
	//auto ShooterCharacter = Cast<AVR_Character>(OtherActor);
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);//Player
	if (OtherActor == Player)
	{
		IsAttacking = true;
		if (EnemyController)//BehaviorTree access through GetBlackboardComponent
		{
			EnemyController->GetBlackboardComponent()->SetValueAsBool(
				TEXT("IsAttacking"),
				true
			);
		}
	}
}

void AEnemyActor::AttackSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr) return;
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);//Player
	if (OtherActor == Player)
	{
		IsAttacking = false;
		if (EnemyController)
		{
			EnemyController->GetBlackboardComponent()->SetValueAsBool(
				TEXT("IsAttacking"),
				false
			);
		}
	}

}



// Called every frame
void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);//Player

	//AVR_Character* VR_Char = Cast<AVR_Character>(PlayerPawn);
	if (PlayerPawn)
	{
		
		if (EnemyController->LineOfSightTo(PlayerPawn))
		{
			SniffingSphereComponent->SetGenerateOverlapEvents(false);//After finding the player, the enemy stops sniffing

			EnemyController->MoveToActor(PlayerPawn, 100);//move to player
			EnemyController->SetFocus(PlayerPawn);
		}
		else
		{
			EnemyController->ClearFocus(EAIFocusPriority::Gameplay);

			SniffingSphereComponent->SetGenerateOverlapEvents(true);
		}

	}
	
}

// Called to bind functionality to input
void AEnemyActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
//Sniffing Sphere distracts the enemy from Patrol Points
void AEnemyActor::SniffingSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
		if (OtherActor == nullptr) return;

		//auto character = Cast<AVR_Character>(OtherActor);//if sphere overlaps the player 
		//if (character)
						
								//(Or)

		APawn* Playercharacter = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);//Player
		if (OtherActor == Playercharacter)
		{
			UGameplayStatics::PlaySoundAtLocation(this, SniffingSound, GetActorLocation());//some immersive sounds

			if (EnemyController)
			{
				EnemyController->GetBlackboardComponent()->SetValueAsObject(TEXT("Enemy"), Playercharacter);//BehaviorTree access through GetBlackboardComponent


			}

			if (EnemyController)
			{
				//(Searching Movements are done by using GetRandomReachablePointInRadius() )

				//(Sniffing Radius with least float value which makes the enemy to Search around almost near the location of the player )

				// to find the player,	it Searches  around almost near the location

				//if the enemy founds the player then it will process the tick function and Attack function

				FVector SniffingVector = UNavigationSystemV1::GetRandomReachablePointInRadius(this, GetActorLocation(), SniffingRadius);

				EnemyController->GetBlackboardComponent()->SetValueAsVector(
					TEXT("SniffingVector"),
					SniffingVector);
			}



		}
	

	


}
void AEnemyActor::SniffingSphereEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
		if (EnemyController)
		{


			FVector SniffingVector = UNavigationSystemV1::GetRandomReachablePointInRadius(this, GetActorLocation(), SniffingRadius);

			EnemyController->GetBlackboardComponent()->SetValueAsVector(
				TEXT("SniffingVector"),
				SniffingVector);
		}

	


}

void AEnemyActor::AttackAnim(FName Section, float PlayRate)//Attack Anim
{
	UAnimInstance* AttackAnimInstance = GetMesh()->GetAnimInstance();
	if (AttackAnimInstance && AttackMontage)
	{
		AttackAnimInstance->Montage_Play(AttackMontage);
		AttackAnimInstance->Montage_JumpToSection(Section, AttackMontage);//Each section has its individual animation
	}
	
}

//Attack Anim Section 
FName AEnemyActor::SectionName()
{
	FName SectionNameAnim;
	const int32 Section{ FMath::RandRange(1,5) };///Randon int switch 
	switch (Section)
	{
	case 1:
		SectionNameAnim = RoarUp;
		
	case 2:
		SectionNameAnim = FastL;
		break;
	case 3:
		SectionNameAnim = FastR;
			break;
	case 4:
		SectionNameAnim = Knock;
			break;

	case 5:
		SectionNameAnim = Four;
			break;
	
	
	}
	return SectionNameAnim;
}

void AEnemyActor::AnimMontage(FName SectionName, float Rate)//Hit React
{
	UAnimInstance* HitReactAnimInstance = GetMesh()->GetAnimInstance();
	if (HitReactAnimInstance)
	{
		HitReactAnimInstance->Montage_Play(HitReactMontage, Rate);
		HitReactAnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);//Each section has its individual animation
	}
}

void AEnemyActor::BulletHit_Implementation(FHitResult Hit)//Interface fumction implementation
{
	if (bulletImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, bulletImpactSound, Hit.Location);

		UGameplayStatics::PlaySoundAtLocation(this, Enemyscreamming, Hit.Location);
		
	}
	if (ImpactParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, Hit.Location, FRotator(0.f), true);
	}
	ShowHealthBar();

	//BulletHit ---> different Animation for each different bones

	if (Hit.BoneName == HeadSocket)
	{

		AnimMontage(FName("HitReact_Front"), 1.0f); // Hit react "AnimMontage" Function called

		UGameplayStatics::PlaySoundAtLocation(this, ImpactOnAttackedBoneSocket, Hit.Location);
	}

	if (Hit.BoneName == BackSocket)
	{

		AnimMontage(FName("HitReact_Back"), 1.0f);

		UGameplayStatics::PlaySoundAtLocation(this, ImpactOnAttackedBoneSocket, Hit.Location);
	}


	if (Hit.BoneName == LeftSocket)
	{

		AnimMontage(FName("HitReact_Left"), 1.0f); 

		UGameplayStatics::PlaySoundAtLocation(this, ImpactOnAttackedBoneSocket, Hit.Location);

		
	}

	if (Hit.BoneName == RightSocket)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactOnAttackedBoneSocket, Hit.Location);
		
		AnimMontage(FName("HitReact_Right"), 1.0f);
	}

	
}


float AEnemyActor::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health - DamageAmount <= 0)
	{
		Health = 0.0f;
		Die();
		GetMesh()->USkeletalMeshComponent::PlayAnimation(Death, false);

		UGameplayStatics::PlaySoundAtLocation(this, DeathShotSound, GetActorLocation());
	}
	else
	{
		Health -= DamageAmount;
	}
	return DamageAmount;

	UE_LOG(LogTemp, Warning, TEXT("Health :%f"), Health);
}





void AEnemyActor::Die()
{
	HideHealthBar();//Hide Widget 
}





void AEnemyActor::ShowHealthBar_Implementation()
{
	GetWorldTimerManager().ClearTimer(HealthBarTime);
	GetWorldTimerManager().SetTimer(HealthBarTime, this, &AEnemyActor::HideHealthBar, HealthBarTimeDisplay); //display to hide widget
}																					//call back fumction


