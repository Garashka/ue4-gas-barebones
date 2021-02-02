// Fill out your copyright notice in the Description page of Project Settings.
/**
 * Common class base between playable hero characters and minion/npc characters
 * Implements an ASC so it can apply abilities, have effects and take damage (where necessary)
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GASCharacterBase.generated.h"

UCLASS()
class GASBAREBONES_API AGASCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGASCharacterBase(const class FObjectInitializer& ObjectInitializer);

	// Implement IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void InitializeGameplayAbilities();

	virtual void AddGameplayAbilities(TArray<TSubclassOf<class UGASGameplayAbilityBase>> AbilitiesToAdd);

	virtual void AddGameplayAbility(TSubclassOf<class UGASGameplayAbilityBase> AbilityToAdd);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Instead of TWeakObjectPtrs, you could just have UPROPERTY() hard references or no references at all and just call
	// GetAbilitySystem() and make a GetAttributeSetBase() that can read from the PlayerState or from child classes.
	// Just make sure you test if the pointer is valid before using.
	// I opted for TWeakObjectPtrs because I didn't want a shared hard reference here and I didn't want an extra function call of getting
	// the ASC/AttributeSet from the PlayerState or child classes every time I referenced them in this base class.

	TWeakObjectPtr<class UGASAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class UGASAttributeSetBase> AttributeSetBase;


	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Abilities")
		TArray<TSubclassOf<class UGASGameplayAbilityBase>> DefaultAbilities;

	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Abilities")
		TSubclassOf<class UGameplayEffect> DefaultAttributes;

	// These effects are only applied one time on startup
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Abilities")
		TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;
};
