// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/Action_ArtifactRewardScreen.h"

#include "CardSystem/CardPlayer.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "UI/UW_ArtifactRewardScreen.h"
#include "UI/UW_Layout_Cos.h"


// Sets default values
AAction_ArtifactRewardScreen::AAction_ArtifactRewardScreen()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AAction_ArtifactRewardScreen::PlayAction_Implementation()
{
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

	CardPlayer->PlayerUI->WBP_ArtifactReward->UpdateReward(Artifact);

	Execute_EndAction(this);
}

