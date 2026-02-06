// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ADummyEnemy.generated.h"

class AMyCharacter;
class UBoxComponent;

UCLASS(Blueprintable)
class BS_UE_PARRYMECHANIC_API AADummyEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AADummyEnemy();

	UFUNCTION(BlueprintCallable) void OpenAttackWindow();
	UFUNCTION(BlueprintCallable) void CloseAttackWindow();
	
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "Combat") void SetIsParryable(bool bParryable) {bIsCurrentlyParryable = bParryable;}
	UFUNCTION(Blueprintable, Category = "Timer") void TestTimerFunction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat") bool bIsCurrentlyParryable = false;
	
	UPROPERTY()
	TArray<AActor*> AlreadyHitPlayers;

	UPROPERTY(EditAnywhere, Category = "Timer") FTimerHandle timerHandle;
	//Debugging
	UPROPERTY(EditAnywhere, Category = "Parry|Debug") bool bShowParryDebug = true;

	UPROPERTY(EditAnywhere, Category = "Parry|Debug") float mDebugSphereHeight = 120.0f;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///Components
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") USkeletalMeshComponent* SkeletalMesh;	//Skeletal Mesh For the dummy
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") UBoxComponent* AttackHitBox;//Collision box used for detecting hits/parry
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") UBoxComponent* MeshHitBox; //Enemy HitBox
	
	//attack animation montage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")UAnimMontage* AttackMontage;
	//is attack currently active
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Attack")bool bAttackActive = false;
	
	//Handles Overlap with player for parry detection
	UFUNCTION()
	void OnAttackOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};
