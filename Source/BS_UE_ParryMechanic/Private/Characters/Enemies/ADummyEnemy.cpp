// Fill out your copyright notice in the Description page of Project Settings.
#include "Characters/Enemies/ADummyEnemy.h"
#include "MyCharacter.h"
#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/EngineTypes.h"
#include "UniversalObjectLocators/AnimInstanceLocatorFragment.h"

// Sets default values
AADummyEnemy::AADummyEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	mDebugSphereHeight = 50.0f;
	//Creating Mesh
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);

	MeshHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MeshHitBox"));
	MeshHitBox->SetupAttachment(RootComponent);
	MeshHitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshHitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	SetRootComponent(MeshHitBox);

	//Create HitBox
	AttackHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackHitBox"));
	AttackHitBox->SetupAttachment(SkeletalMesh);
	AttackHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackHitBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	AttackHitBox->SetGenerateOverlapEvents(true);
	
	//bind Overlap event
	AttackHitBox->OnComponentBeginOverlap.AddDynamic(this,&AADummyEnemy::OnAttackOverlap);
}
// Called when the game starts or when spawned
void AADummyEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	bIsAttacking = true;
	AttackHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision); //setting the box collider to not take collision
}
void AADummyEnemy::EnterStunnedState()
{
	bIsStunned = true;
	bIsAttacking = false;
	
	GetWorldTimerManager().SetTimer(
	StunTimerHandle,
	this,
	&AADummyEnemy::ExitStunnedState,
	StunTime,
	false);
}
void AADummyEnemy::ExitStunnedState()
{
	bIsStunned = false;
	bIsAttacking = true;
}
#pragma region Anim Notify
void AADummyEnemy::OpenAttackWindow() //opening the parry window for the play
{
	AlreadyHitPlayers.Empty();
	AttackHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	bIsCurrentlyParryable = true;
}
void AADummyEnemy::CloseAttackWindow() //closing the parry window for the player
{
	bIsCurrentlyParryable = false;
	AttackHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
#pragma endregion
#pragma region Attack/Parry detection
void AADummyEnemy::OnAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{ //called when  overlapped with this collider
	if (!bIsAttacking || AlreadyHitPlayers.Contains(OtherActor)) return; //stops the function if tjhe attack is not active or actor was already hit
	
	UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance(); //gets the dummies skeletal mesh then gets anim instance off it
	if (AMyCharacter* Player = Cast<AMyCharacter>(OtherActor)) //checking if the other actor is the AMyCharacter
	{
		//checking if the player is parrying and if the parry animation is playing
		if (bIsCurrentlyParryable && Player->GetMesh()->GetAnimInstance()->Montage_IsPlaying(Player->GetAnimationMontage()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Parried Successfully"));
			CloseAttackWindow(); //closing the attack disabling hitbox component
			AlreadyHitPlayers.Add(OtherActor); //mark player as already hit 
			EnterStunnedState();
			
			return;
		}
		if (!bIsCurrentlyParryable) //if player failed the parry
		{
			Player->HealthChange(10);
			AlreadyHitPlayers.Add(Player); //logs that the player was hit
			UE_LOG(LogTemp, Warning, TEXT("Player Parry Failed"));
		}
	}
}
#pragma endregion
#pragma region Debug
void AADummyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bShowParryDebug)
	{
		if (bIsAttacking) //while the Attack is able to be parried draw a box to show the parry zone
		{
			FColor BoxColor = bIsAttacking ? FColor::Green : FColor::Red;
			DrawDebugBox(
				GetWorld(),
				AttackHitBox->GetComponentLocation(),
				AttackHitBox->GetScaledBoxExtent(),
				BoxColor,
				false,
				0.0f,
				0,
				2.0f
			);
		}
		if (bShowParryDebug) //while the Attack is able to be parried draw a Sphere above the head to show it can be parried
		{
			FVector SphereLocation = GetActorLocation() + FVector(0.0f, 0.0f, mDebugSphereHeight);
			FColor SphereColor = bIsAttacking ? FColor::Green : FColor::Red;
			
			DrawDebugSphere(
			GetWorld(),
			SphereLocation,
			30.0f,
			12,
			SphereColor,
			false,
			0.0f,
			0,
			2.0f
			);
		}
	}
}
#pragma endregion