// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GASCharacterBase.h"
#include "AbilitySystem/GASAbilitySystemComponent.h"
#include "AbilitySystem/GASGameplayAbilityBase.h"


AGASCharacterBase::AGASCharacterBase(const class FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	bAlwaysRelevant = true;

}

UAbilitySystemComponent* AGASCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

// Called when the game starts or when spawned
void AGASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGASCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGASCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


}

void AGASCharacterBase::InitializeGameplayAbilities()
{
	// Only allow on server
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->bCharacterAbilitiesInitialized)
	{
		return;
	}
	AddGameplayAbilities(DefaultAbilities);

	AbilitySystemComponent->bCharacterAbilitiesInitialized = true;
}

void AGASCharacterBase::AddGameplayAbilities(TArray<TSubclassOf<class UGASGameplayAbilityBase>> AbilitiesToAdd)
{
	// Only allow on server
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() )
	{
		return;
	}
	for (TSubclassOf<UGASGameplayAbilityBase>& Ability : AbilitiesToAdd)
	{
		AddGameplayAbility(Ability);
	}
}

void AGASCharacterBase::AddGameplayAbility(TSubclassOf<class UGASGameplayAbilityBase> AbilityToAdd)
{
	// Only allow on server
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid())
	{
		return;
	}

	int32 AbilityLevel = 1;
	int32 InputID = static_cast<int32>(AbilityToAdd.GetDefaultObject()->AbilityInputID);

	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityToAdd, AbilityLevel, InputID, this));
}

