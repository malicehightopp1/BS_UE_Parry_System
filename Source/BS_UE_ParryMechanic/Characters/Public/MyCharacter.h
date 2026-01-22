// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MyCharacter.generated.h"

UCLASS()
class BS_UE_PARRYMECHANIC_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();
	void AttemptParry();
	void OpenParryWindow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parry")
	float mParryWindowDuration = 0.25f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Parry")
	bool bParryWindowOpen = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Parry")
	bool bIsParrying = false;

	FTimerHandle ParryWindowTimer;

	void CloseParryWindow();

	//Debugging
	UPROPERTY(EditAnywhere, Category = "Parry|Debug")
	bool bShowParryDebug = true;

	UPROPERTY(EditAnywhere, Category = "Parry|Debug")
	float mDebugSphereHeight = 120.0f;

	//Enhanced Input

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* CombatMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ParryAction;

	void OnParryInput(const FInputActionValue& Value);
};
