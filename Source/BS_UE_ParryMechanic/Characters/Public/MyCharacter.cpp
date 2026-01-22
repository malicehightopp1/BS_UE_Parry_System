// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMyCharacter::BeginPlay() //basically 1. when the game starts do normal setup 2.check if the character is controlled by a player 
                              //3. if yes, get that players enhanced input system 4. Add the combat controls to the input system so the payer can use them
{
	Super::BeginPlay();
	
	if (APlayerController* PC = Cast<APlayerController>(Controller)) //seeing if the controller is a player and if it is stores it in the PC variable
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =  //since it is a player it asks the player controller for the enhanced input subsystems
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>( //get the actual player object that exists
				PC->GetLocalPlayer()))  //then get the UEnhancedInputLocalPlayerSubsystem which handles input mapping
		{
			Subsystem->AddMappingContext(CombatMappingContext, 1); //adding mapping context for input
		}
	}
}
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent); //init setup by unreal getting the character ready to receive input
	
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent)) //converting input component to the enhanced input version
	{
		EnhancedInput->BindAction( //binding the parry action to the enhanced input component
		ParryAction,
		ETriggerEvent::Started,
		this,
		&AMyCharacter::OnParryInput
		);
	}
}
void AMyCharacter::OnParryInput(const FInputActionValue& Value)
{
	AttemptParry();
	
	if (bShowParryDebug)
	{
		FVector SphereLocation = GetActorLocation() + FVector(0.0f, 0.0f, mDebugSphereHeight);
		DrawDebugSphere(GetWorld(), SphereLocation, 30.0f, 12, FColor::Green, false, 1.0f);
	}
}
void AMyCharacter::AttemptParry()
{
	if (bParryWindowOpen)
	{
		bParryWindowOpen = true;
		//for parry logic
	}
}