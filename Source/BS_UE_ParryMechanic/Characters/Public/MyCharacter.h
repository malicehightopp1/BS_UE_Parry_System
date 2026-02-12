// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MyCharacter.generated.h"

//class UProgressBar;
class USpringArmComponent;
class UCameraComponent;
class AMyCharacter;

UCLASS()
class BS_UE_PARRYMECHANIC_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter(); //constructor
	virtual void Tick(float DeltaSeconds) override;

	UAnimMontage* GetAnimationMontage() const;
	UFUNCTION(BlueprintCallable, Category = "Player | Player Health") float GetHealthPercent() const { return CurrenHealth / MaxHealth; }
	UFUNCTION(BlueprintCallable, Category = "Player | Player Health") void HealthChange(float HealthToLose); //public so dummy can access and variable private so can only call this function
	
	UFUNCTION(BlueprintCallable, Category = "Player | Player Stamina") float GetStaminaPercent() const { return CurrentStamina / MaxStamina; }
private:
		void AttemptParry();
		
		UFUNCTION(BlueprintCallable, Category = "Player | ParrySystem") void SetParryWindow(bool bIsOpen);
		UFUNCTION(BlueprintCallable, Category = "Player | ParrySystem") bool IsParryWindowOpen() const {return bParryWindowOpen;}; //called at the edn of the anim and closes the parry
		
	//Stamina
		UFUNCTION(BlueprintCallable, Category = "Player | Player Stats |Player Stamina") void StaminaChange(float StaminaToLose);
		UFUNCTION(blueprintCallable, Category = "Player | Player Stats |Player Stamina") void StaminaRegenHandle(float DeltaSeconds);
		UPROPERTY(EditDefaultsOnly, Category = "Player | Player Stats | Player Stamina") float StaminaRegenAmount = 10.0f;
		UPROPERTY(EditDefaultsOnly, Category = "Player | Player Stats | Player Stamina") float StaminaRegenInterval = 1.0f;
		UPROPERTY(EditDefaultsOnly, Category = "Player | Player Stats | Player Stamina") float StaminaRegenTimer = 0.f;
		
		UPROPERTY(EditAnywhere, Category = "Player | Animations" ) UAnimMontage* AM_Parry; //needs to be accessable to the dummy to check if animation is playing
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;
		virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Player stats
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
		//Player Health 
		UPROPERTY(EditDefaultsOnly, Category = "Player | Player Stats | Player Health") float CurrenHealth;
		UPROPERTY(EditDefaultsOnly, Category = "Player | Player Stats | Player Health") float MaxHealth;
		
		//Player Stamina
		UPROPERTY(EditDefaultsOnly, Category = "Player | Player Stats | Player Stamina") float CurrentStamina;
		UPROPERTY(EditDefaultsOnly, Category = "Player | Player Stats | Player Stamina") float MaxStamina;
		
		UPROPERTY(VisibleAnywhere, Category = "Player | ParrySystem") bool bParryWindowOpen = false;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Input actions
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		UPROPERTY(EditDefaultsOnly, Category = "Player | Player Setup | Input") //context mapping
		class UInputMappingContext* IMC_Default;
		
		UPROPERTY(EditDefaultsOnly, Category = "Player | Player Setup | Input") //movement input action
		class UInputAction* IA_Move;
		
		UPROPERTY(EditDefaultsOnly, Category = "Player | Player Setup | Input") //look input action
		class UInputAction* IA_Look;
		
		UPROPERTY(EditDefaultsOnly, Category = "Player | Player Setup | Input") //parry input action
		class UInputAction* ParryAction;
		
		void OnParryInput(const FInputActionValue& Value);
		void Move(const FInputActionValue& Value);	
		void Look(const FInputActionValue& Value);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Camera Setup
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
		UPROPERTY(EditDefaultsOnly, Category = "Player | Player Setup | Camera") //spring arm
		USpringArmComponent* CameraSpringArm;
		
		UPROPERTY(EditDefaultsOnly, Category = "Player | Player Setup | Camera") //follow camera
		UCameraComponent* FollowCamera;
};
