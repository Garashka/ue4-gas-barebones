// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GASPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class GASBAREBONES_API AGASPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AGASPlayerState();

	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UGASAttributeSetBase* GetAttributeSetBase() const;

protected:
	UPROPERTY()
		class UGASAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
		class UGASAttributeSetBase* AttributeSetBase;
};
