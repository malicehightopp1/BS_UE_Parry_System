// Fill out your copyright notice in the Description page of Project Settings.
#include "Characters/Enemies/ADummyEnemy.h"
#include "MyCharacter.h"
#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Engine/EngineTypes.h"

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
	
	AttackHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void AADummyEnemy::OpenAttackWindow()
{
	bAttackActive = true;
	
	AlreadyHitPlayers.Empty();
	AttackHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
void AADummyEnemy::CloseAttackWindow()
{
	bAttackActive = false;
	AttackHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void AADummyEnemy::OnAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bAttackActive) return;
	if (AMyCharacter* Player = Cast<AMyCharacter>(OtherActor))
	{
		if (Player->IsParryWindowOpen())
		{
			UE_LOG(LogTemp,Warning,TEXT("Parry Success"));
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("Parry Failed"));
		}
	}
}
void AADummyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bAttackActive)
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
	
	if (bShowParryDebug)
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