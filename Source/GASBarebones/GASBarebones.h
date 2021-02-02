// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGASAbilityInputID : uint8
{
	// 0 None
	None				UMETA(DisplayName = "None"),
	// 1 Confirm
	Confirm				UMETA(DisplayName = "Confirm"),
	// 2 Cancel
	Cancel				UMETA(DisplayName = "Cancel"),
	// 3 Sprint
	Sprint				UMETA(DisplayName = "Sprint"),
	// 4 Jump
	Jump				UMETA(DisplayName = "Jump"),
	// 5 PrimaryAttack
	PrimaryAttack		UMETA(DisplayName = "Primary Attack"),
	// 6 SecondaryAttack
	SecondaryAttack		UMETA(DisplayName = "Secondary Attack"),
	// 7 Block
	Block				UMETA(DisplayName = "Block"),
	// 8 Evade
	Evade				UMETA(DisplayName = "Evade"),
	// 9 Reload
	Reload				UMETA(DisplayName = "Reload"),
	// 10 Ability 1
	Ability1			UMETA(DisplayName = "Ability1"),
	// 11 Ability 2
	Ability2			UMETA(DisplayName = "Ability2"),
	// 12 Ability 3
	Ability3			UMETA(DisplayName = "Ability3"),
	// 13 Ability 4
	Ability4			UMETA(DisplayName = "Ability4"),
	// 14 Ability 5
	Ability5			UMETA(DisplayName = "Ability5"),
	// 15 Ability 6
	Ability6			UMETA(DisplayName = "Ability6")
};