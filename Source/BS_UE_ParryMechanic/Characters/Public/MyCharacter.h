// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MyCharacter.generated.h"

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
	void AttemptParry();

	UFUNCTION(BlueprintCallable, Category = "ParrySystem") void SetParryWindow(bool bIsOpen);
	UFUNCTION(BlueprintCallable, Category = "ParrySystem") bool IsParryWindowOpen() const {return bParryWindowOpen;}; //called at the edn of the anim and closes the parry 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parry" ) UAnimMontage* AM_Parry; //needs to be accessable to the dummy to check if animation is playing
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Parry")
	bool bParryWindowOpen = false;
	
	

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Input actions and character setup
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditDefaultsOnly, Category = "Input") //context mapping
	class UInputMappingContext* IMC_Default;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input") //movement input action
	class UInputAction* IA_Move;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input") //look input action
	class UInputAction* IA_Look;
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera") //spring arm
	USpringArmComponent* CameraSpringArm;
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera") //follow camera
	UCameraComponent* FollowCamera;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input") //parry input action
	class UInputAction* ParryAction;

	void OnParryInput(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);	
	void Look(const FInputActionValue& Value);
};
