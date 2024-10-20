﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusSystem/Artifacts/Status_Artifact_HeavyTome.h"

#include "CardSystem/CardPlayer.h"
#include "Libraries/AssetPath.h"
#include "StatusSystem/Status_Draw.h"
#include "StatusSystem/Status_Mana.h"
#include "StatusSystem/Status_ManaGain.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


// Sets default values for this component's properties
UStatus_Artifact_HeavyTome::UStatus_Artifact_HeavyTome()
{
	StatusValue = 0;

	SlotType = EStatusSlot::Artifact;

	static ConstructorHelpers::FObjectFinder<UTexture2D> T_HeavyTome(*AssetPath::Texture::T_HeavyTome);
	if(T_HeavyTome.Succeeded())
	{
		Icon = T_HeavyTome.Object;
	}
	else
	{
		// COS_SCREEN(TEXT("T_LuchadorMask를 로드할 수 없습니다."));
	}

	Tint = FLinearColor(0.58f,0.17f,1.0f);

	bCanBeZero = true;

	
	
	FToolTipValue ToolTipValue;
	ToolTipValue.bValued=true;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Tooltips_Artifacts(*AssetPath::DataTable::DT_Tooltips_Artifacts);
	if(DT_Tooltips_Artifacts.Succeeded())
	{
		ToolTipValue.ToolTipTable.DataTable = DT_Tooltips_Artifacts.Object;
	}
	else
	{
		COS_SCREEN(TEXT("DT_Tooltips_Artifacts를 로드할 수 없습니다."));
	}
	
	ToolTipValue.ToolTipTable.RowName = TEXT("HeavyTome");
	Tooltips.Add(ToolTipValue);

	FriendlyName = FText::FromString(TEXT("HeavyTome"));
	
	bShowImmediately = false;
	MaxTriggersPerTick=10;
	CurrentTriggersThisTick=0;
	
	bMaxAble=false;
	MaxValue=0;
	bArtifact = true;
	bInterrupt = false;
}

int32 UStatus_Artifact_HeavyTome::AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon,
	bool bRefreshAppearance, UObject* InPayload)
{
	int32 OldValue = StatusValue;
	
	int32 ParentValue = Super::AddStatusValue(InAmount, bShowSplashNumber, bShowSplashIcon, bRefreshAppearance, InPayload);

	if(ACardPlayer* CardPlayer = Cast<ACardPlayer>(GetOwner()))
	{
		IInterface_CardTarget::Execute_AddToStatus(GetOwner(),UStatus_ManaGain::StaticClass(),ParentValue - OldValue,false,nullptr);

		IInterface_CardTarget::Execute_SubtractFromStatus(GetOwner(),UStatus_Draw::StaticClass(),1,false,nullptr);
	}

	return ParentValue;
}




