// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h" //my class definition

#include "EnhancedInputComponent.h" //binds animations
#include "EnhancedInputSubSystems.h" //Add inputs mapping context
#include "DrawDebugHelpers.h" //drawing debug sphere
#include "Animation/AnimInstance.h" //play animations
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
	//Creating Spring arm
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->TargetArmLength = 300.0f;
	CameraSpringArm->bUsePawnControlRotation = true;
	
	//Creating camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraSpringArm);
	FollowCamera->bUsePawnControlRotation = false;
	
	//character rotation 
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	
	//Movement Rotation 
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f,540.0f,0.0f);
}
#pragma region Input Setup
void AMyCharacter::BeginPlay() //basically 1. when the game starts do normal setup 2.check if the character is controlled by a player 
                              //3. if yes, get that players enhanced input system 4. Add the combat controls to the input system so the payer can use them
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("My character Begin play"));
	APlayerController* PC = Cast<APlayerController>(Controller);
	if (PC)//seeing if the controller is a player and if it is stores it in the PC variable
	{
		ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
		if (LocalPlayer)
		{
			UEnhancedInputLocalPlayerSubsystem* Subsystem = 
				LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			if (Subsystem && IMC_Default)
			{
				Subsystem->AddMappingContext(IMC_Default, 0);
				UE_LOG(LogTemp, Warning, TEXT("Added Mapping Context"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Mapping Context Not Added"));
			}
		}
	}
}
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent); //init setup by unreal getting the character ready to receive input
	
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent)) //converting input component to the enhanced input version
	{
		//binding the parry action to the enhanced input component
		if (ParryAction)
		{
			EnhancedInput->BindAction( ParryAction,ETriggerEvent::Started,this,&AMyCharacter::OnParryInput);
		}
		if (IA_Move)
		{
			EnhancedInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		}
		if (IA_Look)
		{
			EnhancedInput->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
		}
	}
}
#pragma endregion
#pragma region Parry 
void AMyCharacter::OnParryInput(const FInputActionValue& Value)
{
	AttemptParry();
}
void AMyCharacter::AttemptParry()
{
	UE_LOG(LogTemp, Warning, TEXT("Attempt Parry"));
	if (!AM_Parry) return; //Do we have a parry animation?
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); //get animation instance
	if (!AnimInstance) return;
	
	if (AnimInstance -> Montage_IsPlaying(AM_Parry)) //prevent spamming of parry animation
	{
		return; //could try to add timer to make it a bigger delay but this works fine right now
	}
	//Play the parry animation
	if (bParryWindowOpen)
	{
		UE_LOG(LogTemp, Warning, TEXT("Parry Successful"));
		PlayAnimMontage(AM_Parry);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Parry Failed"));
	}
}
void AMyCharacter::OpenParryWindow()
{
	bParryWindowOpen = true;
	UE_LOG(LogTemp, Warning, TEXT("Parry window Open"));
}
void AMyCharacter::CloseParryWindow()
{
	bParryWindowOpen = false;
	UE_LOG(LogTemp, Warning, TEXT("Parry window closed"));
}
#pragma endregion
#pragma region Movement
void AMyCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}
#pragma endregion
#pragma region Look
void AMyCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
#pragma endregion
