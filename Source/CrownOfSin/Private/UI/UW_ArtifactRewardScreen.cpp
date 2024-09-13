// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_ArtifactRewardScreen.h"

#include "UI/UW_StatusIcon.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"

UUW_ArtifactRewardScreen::UUW_ArtifactRewardScreen(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
}

void UUW_ArtifactRewardScreen::UpdateReward(UStatusComponent* Status)
{
	Artifact = Status;

	if(!WBP_StatusIcon->GetClass()->ImplementsInterface(UInterface_StatusIcon::StaticClass()))
	{
		COS_SCREEN(TEXT("WBP_StatusIcon이 Interface_StatusIcon을 상속받지 않았습니다."));
		return;
	}

	IInterface_StatusIcon::Execute_InitStatusWidget(WBP_StatusIcon,Artifact,CosGameTags::StatusBar_RewardBar,true);

	SetVisibility(ESlateVisibility::Visible);

	PlayAnimation(FadeIn);
}
