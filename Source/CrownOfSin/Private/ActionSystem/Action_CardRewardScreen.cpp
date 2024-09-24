// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/Action_CardRewardScreen.h"

#include "CardSystem/CardPlayer.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "UI/UW_CardRewardScreen.h"
#include "UI/UW_Layout_Cos.h"


// Sets default values
AAction_CardRewardScreen::AAction_CardRewardScreen()
{
	bAllowSkip = false;
}

void AAction_CardRewardScreen::PlayAction_Implementation()
{
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

	CardPlayer->PlayerUI->WBP_CardRewardScreen->OnReturnSelectedCardInRewardScreen.AddDynamic(this, &AAction_CardRewardScreen::ContinueActionOnCardSelect);

	CardPlayer->PlayerUI->WBP_CardRewardScreen->UpdateRewardScreen(CardOptions,ScreenText,bAllowSkip);
}

void AAction_CardRewardScreen::ContinueActionOnCardSelect(bool bSkipped, ACardBase* InCard)
{
	EndAction();
}


