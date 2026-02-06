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
	RootComponent = SkeletalMesh;
	
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
	
	AttackHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision); //setting the box collider to not take collision
}
void AADummyEnemy::OpenAttackWindow() //opening the parry window for the play
{
	bAttackActive = true;
	AlreadyHitPlayers.Empty();
	AttackHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	bIsCurrentlyParryable = true;
}
void AADummyEnemy::CloseAttackWindow() //closing the parry window for the player
{
	bAttackActive = false;
	bIsCurrentlyParryable = false;
	AttackHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void AADummyEnemy::OnAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bAttackActive || AlreadyHitPlayers.Contains(OtherActor)) return;

	USkeletalMeshComponent* MeshComp = FindComponentByClass<USkeletalMeshComponent>();
	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (AMyCharacter* Player = Cast<AMyCharacter>(OtherActor))
	{
		//checking if the player is parrying and if the parry animation is playing
		if (bIsCurrentlyParryable && Player->GetMesh()->GetAnimInstance()->Montage_IsPlaying(Player->AM_Parry))
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Parried Successfully"));
			AnimInstance->Montage_Stop(0.2f, AttackMontage); //Stopping enemy animation if parried succesfully
			CloseAttackWindow(); //closing the attack 
			AlreadyHitPlayers.Add(OtherActor);
			
			return;
		}
		if (!bIsCurrentlyParryable) //if player failed the parry
		{
			AlreadyHitPlayers.Add(Player);
			UE_LOG(LogTemp, Warning, TEXT("Player Parry Failed"));
		}
	}

}
void AADummyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bAttackActive) //while the Attack is able to be parried draw a box to show the parry zone
	{
		DrawDebugBox(
			GetWorld(),
			AttackHitBox->GetComponentLocation(),
			AttackHitBox->GetScaledBoxExtent(),
			FColor::Green,
			false,
			0.0f,
			0,
			2.0f
		);
	}
	
	if (bShowParryDebug) //while the Attack is able to be parried draw a Sphere above the head to show it can be parried
	{
		FVector SphereLocation = GetActorLocation() + FVector(0.0f, 0.0f, mDebugSphereHeight);
		FColor SphereColor = bAttackActive ? FColor::Green : FColor::Red;
		
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