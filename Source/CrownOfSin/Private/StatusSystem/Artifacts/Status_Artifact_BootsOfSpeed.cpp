// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusSystem/Artifacts/Status_Artifact_BootsOfSpeed.h"

#include "CardSystem/CardPlayer.h"
#include "Interfaces/Interface_CardTarget.h"
#include "Libraries/AssetPath.h"
#include "StatusSystem/Status_Draw.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


class ACardPlayer;
// Sets default values for this component's properties
UStatus_Artifact_BootsOfSpeed::UStatus_Artifact_BootsOfSpeed()
{
	StatusValue = 0;

	SlotType = EStatusSlot::Artifact;

	static ConstructorHelpers::FObjectFinder<UTexture2D> T_BootsOfSpeed(*AssetPath::Texture::T_BootsOfSpeed);
	if(T_BootsOfSpeed.Succeeded())
	{
		Icon = T_BootsOfSpeed.Object;
	}
	else
	{
		COS_SCREEN(TEXT("T_BootsOfSpeed를 로드할 수 없습니다."));
	}

	Tint = FLinearColor(1.0f,1.0f,1.0f);

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
	
	ToolTipValue.ToolTipTable.RowName = TEXT("BootsOfSpeed");
	Tooltips.Add(ToolTipValue);

	FriendlyName = FText::FromString(TEXT("Boots Of Speed"));
	
	bShowImmediately = false;
	MaxTriggersPerTick=10;
	CurrentTriggersThisTick=0;
	bMaxAble=false;
	MaxValue=0;
	bArtifact = true;
	bInterrupt = false;
}

int32 UStatus_Artifact_BootsOfSpeed::AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon,
	bool bRefreshAppearance, UObject* InPayload)
{
	int32 OldValue = StatusValue;
	
	int32 ParentValue =  Super::AddStatusValue(InAmount, bShowSplashNumber, bShowSplashIcon, bRefreshAppearance, InPayload);

	if(ACardPlayer* CardPlayer = Cast<ACardPlayer>(GetOwner()))
	{
		IInterface_CardTarget::Execute_AddToStatus(GetOwner(),UStatus_Draw::StaticClass(),ParentValue - OldValue,false,nullptr);
	}

	return ParentValue;
}



