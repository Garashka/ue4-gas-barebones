// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GASPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Player/GASPlayerState.h"
#include "AbilitySystem/GASAbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GASBarebones/GASBarebones.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystem/AttributeSets/GASAttributeSetBase.h"

AGASPlayerCharacter::AGASPlayerCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeLocation(FVector(0, 0, 68.492264));
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	//FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	FollowCamera->FieldOfView = 80.0f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AGASPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AGASPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGASPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGASPlayerCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AGASPlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGASPlayerCharacter::LookUpAtRate);

	BindASCInput();
}

// Called when player joins or on respawn. May need to change behaviour if we want players to retain previous stats when they reconnect
void AGASPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AGASPlayerState* PS = GetPlayerState<AGASPlayerState>();
	if (PS)
	{
		AbilitySystemComponent = Cast<UGASAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		// Store references to Attribute Sets 
		AttributeSetBase = PS->GetAttributeSetBase();

		// Initialize ASC components from BP
		/*InitializeAttributes();
		AddStartupEffects();*/
		InitializeGameplayAbilities();

		//if (AbilitySystemComponent->GetTagCount(DeadTag) > 0)
		//{
		//	// Reset important values here before removing dead tag so player doesn't immediately die again ;) 
		//	SetHealth(GetMaxHealth());
		//}

		// Force DeadTagCount to 0 for respawns
		//AbilitySystemComponent->SetTagMapCount(DeadTag, 0);
	}
}

void AGASPlayerCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
}

void AGASPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AGASPlayerState* PS = GetPlayerState<AGASPlayerState>();
	if (PS)
	{
		// Set ASC on client. Server does in PossessedBy
		AbilitySystemComponent = Cast<UGASAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// Init ASC Actor Info on clients. Server does in PossessedBy
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		// Bind player input to ASC. Also called in SetupPlayerInputComponent due to potential race conditions
		BindASCInput();

		AttributeSetBase = PS->GetAttributeSetBase();

		//InitializeAttributes();

		//if (AbilitySystemComponent->GetTagCount(DeadTag) > 0)
		//{
		//	// Reset values when respawning. It will also be called by the server which will eventually override this
		//	// but calling it here will make it more responsive
		//	SetHealth(GetMaxHealth());
		//}
	}
}

bool AGASPlayerCharacter::IsAlive()
{
	//return GetHealth() > 0.0f;
	return true;
}

void AGASPlayerCharacter::BindASCInput()
{
	if (!bASCInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"), FString("EGASAbilityInputID"), static_cast<int32>(EGASAbilityInputID::Confirm), static_cast<int32>(EGASAbilityInputID::Cancel)));

		bASCInputBound = true;
	}
}

void AGASPlayerCharacter::MoveForward(float Value)
{
	if (IsAlive())
	{
		AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
	}
}

void AGASPlayerCharacter::MoveRight(float Value)
{
	if (IsAlive())
	{
		AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
	}
}

void AGASPlayerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGASPlayerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
