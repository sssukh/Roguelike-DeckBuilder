// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_CardVisual.h"

UUW_CardVisual::UUW_CardVisual(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
}

void UUW_CardVisual::AnimateCardAppear_Implementation(EUMGSequencePlayMode::Type PlayMode)
{
	PlayAnimation(Appear,0.0,1,PlayMode);
}

void UUW_CardVisual::UpdateCardWidget_Implementation(ACardBase* CardActor)
{
	
}
