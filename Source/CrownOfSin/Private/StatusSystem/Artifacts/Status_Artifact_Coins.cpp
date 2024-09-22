// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusSystem/Artifacts/Status_Artifact_Coins.h"

#include "Libraries/AssetPath.h"
#include "Utilities/CosLog.h"


// Sets default values for this component's properties
UStatus_Artifact_Coins::UStatus_Artifact_Coins()
{
	StatusValue = 0;
	SlotType = EStatusSlot::Currency;
	static ConstructorHelpers::FObjectFinder<UTexture2D> T_Coins(*AssetPath::Texture::T_Coins);
	if(T_Coins.Succeeded())
	{
		Icon = T_Coins.Object;
	}
	else
	{
		COS_SCREEN(TEXT("T_Coins를 로드할 수 없습니다."));
	}
	Tint = FLinearColor::Yellow;
	TextAlignment = EStatusTextAlignment::BottomRight;
	Priority = 0.0f;
	bCanBeZero = true;
	IncomingStatusChange = 0;
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
	
	ToolTipValue.ToolTipTable.RowName = TEXT("Coins");
	Tooltips.Add(ToolTipValue);
	FriendlyName = FText::FromString(TEXT("Lollipop"));
	bShowImmediately = false;
	MaxTriggersPerTick=10;
	CurrentTriggersThisTick=0;
	bMaxAble=false;
	MaxValue=0;
	bArtifact = true;
	bInterrupt = false;
}



