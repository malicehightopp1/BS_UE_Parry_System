// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
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
		UFUNCTION(BlueprintCallable, Category = "Dummy | Combat | Attack") void SetIsParryable(bool bParryable) {bIsAttacking = bParryable;}
	
		UPROPERTY(BlueprintReadOnly, Category = "Dummy | Combat | Stun") bool bIsStunned = false;
		UPROPERTY(BlueprintReadOnly, Category = "Dummy | Combat | Attack")bool bIsAttacking = false;
		UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Dummy | Combat | Charge") bool bIsCharging = false;
		UFUNCTION(BlueprintCallable, Category = "Dummy | ParrySystem | Particles") void SpawnParticles();
	
protected:
		UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Dummy | Dummy Setup | Components") USkeletalMeshComponent* SkeletalMesh;	//Skeletal Mesh For the dummy
	
private:
		virtual void BeginPlay() override;

		UPROPERTY(EditDefaultsOnly, Category = "Dummy | ParrySystem | Particles") UNiagaraSystem* ParryEffect;
		UPROPERTY(EditDefaultsOnly, Category = "Dummy | ParrySystem | Particles") UBoxComponent* ParticleBox;
		UPROPERTY(EditDefaultsOnly, Category = "Dummy | ParrySystem | Particles") FVector ParticleLocation;
		UPROPERTY(EditDefaultsOnly, Category = "Dummy | ParrySystem | Particles") FRotator ParticleRotation;
		void EnterStunnedState();
		void ExitStunnedState();
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Combat
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
		UPROPERTY(VisibleAnywhere, Category = "Dummy | Combat | Attack") bool bIsCurrentlyParryable = false;
		//is attack currently active
		UPROPERTY()
		TArray<AActor*> AlreadyHitPlayers;
		//stun 
		UPROPERTY(EditAnywhere, Category = "Dummy | Combat | Stun") float StunTime = 5.0f;
		FTimerHandle StunTimerHandle;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///Components
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		UPROPERTY(VisibleAnywhere, Category = "Dummy | Dummy Setup | Components") UBoxComponent* AttackHitBox;//Collision box used for detecting hits/parry
		UPROPERTY(VisibleAnywhere, Category = "Dummy | Dummy Setup | Components") UBoxComponent* MeshHitBox; //Enemy HitBox
		
		
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
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///Debugging
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
		UPROPERTY(EditAnywhere, Category = "Dummy | Parry | Debug") bool bShowParryDebug = true;

		UPROPERTY(EditAnywhere, Category = "Dummy | Parry | Debug") float mDebugSphereHeight = 120.0f;
};
