// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GASPlayerController.h"
#include "Player/GASPlayerState.h"
#include "AbilitySystemComponent.h"

void AGASPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AGASPlayerState* PS = GetPlayerState<AGASPlayerState>();
	if (PS)
	{
		// Init ASC with PS (Owner) and our new Pawn (AvatarActor)
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);

	}
}

void AGASPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}
