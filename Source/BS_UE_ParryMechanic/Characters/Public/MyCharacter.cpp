// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h" //my class definition

#include "EnhancedInputComponent.h" //binds animations
#include "EnhancedInputSubSystems.h" //Add inputs mapping context
#include "DrawDebugHelpers.h" //drawing debug sphere
#include "Animation/AnimInstance.h" //play animations
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	AutoPossessPlayer = EAutoReceiveInput::Player0; //making the player 0 the main character and the player that recieves input
	
	//Creating Spring arm
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom")); //creating the spring arm
	CameraSpringArm->SetupAttachment(RootComponent); //attaching spring arm to the root component
	CameraSpringArm->TargetArmLength = 300.0f; //setting the length of spring arm to 300
	CameraSpringArm->bUsePawnControlRotation = true;

	//hitbox setup
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	
	//Creating camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera")); //creating camera
	FollowCamera->SetupAttachment(CameraSpringArm); //attaching it to spring arm
	FollowCamera->bUsePawnControlRotation = false; //making it so the camera rotation follows its own rotation not the pawns
	
	//character rotation - used for the camera to have it not use the pawns rotation for the camera 
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
	
	APlayerController* PC = Cast<APlayerController>(Controller); //casting the player controller to a player controller just making sure the player controller is a player controller
	if (PC)//seeing if the controller is a player and if it is stores it in the PC variable
	{
		ULocalPlayer* LocalPlayer = PC->GetLocalPlayer(); //Gets the local player instacne - could be made into if statement /////ASK TEACHER IF ITS BETTER///////
		if (LocalPlayer)
		{
			UEnhancedInputLocalPlayerSubsystem* Subsystem =  
				LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(); //accesses unreal's enhanced input system 1. for input mapping context 2. input actions 
			if (Subsystem && IMC_Default) //if the subsystem exists and if the input mapping context is in the details tab
			{
				Subsystem->AddMappingContext(IMC_Default, 0); //activates the input mapping and sets the priority
				UE_LOG(LogTemp, Warning, TEXT("Added Mapping Context"));
			}
			else //for debugging purposes *REMOVE LATER*
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
		if (ParryAction)//binding the parry action to the enhanced input component
		{
			EnhancedInput->BindAction( ParryAction,ETriggerEvent::Started,this,&AMyCharacter::OnParryInput);//started because its only played at the start
		}
		if (IA_Move)//binding the Move action to the enhanced input component
		{
			EnhancedInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AMyCharacter::Move);//triggered because its fired every frame
		}
		if (IA_Look)//binding the Look action to the enhanced input component
		{
			EnhancedInput->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AMyCharacter::Look);//triggered because its fired every frame
		}
	}
}
#pragma endregion
#pragma region Parry 
void AMyCharacter::OnParryInput(const FInputActionValue& Value) //on input of the parry input play function
{
	AttemptParry();
}
void AMyCharacter::AttemptParry()
{
	UE_LOG(LogTemp, Warning, TEXT("Attempt Parry"));
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); //get animation instance
	if (AnimInstance && AM_Parry && !AnimInstance -> Montage_IsPlaying(AM_Parry)) //prevent spamming of parry animation
	{
		PlayAnimMontage(AM_Parry); //play the parry animation
	}
}
void AMyCharacter::SetParryWindow(bool bIsOpen)
{
	bParryWindowOpen = bIsOpen;
	UE_LOG(LogTemp, Warning, TEXT("Set Parry Window"));
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
