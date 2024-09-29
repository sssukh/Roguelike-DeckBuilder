// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusSystem/Artifacts/Status_Artifact_Lollipop.h"
#include "Libraries/AssetPath.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


// Sets default values for this component's properties
UStatus_Artifact_Lollipop::UStatus_Artifact_Lollipop()
{

	StatusValue = 0;

	SlotType = EStatusSlot::Artifact;

	static ConstructorHelpers::FObjectFinder<UTexture2D> T_Lollipop(*AssetPath::Texture::T_Lollipop);
	if(T_Lollipop.Succeeded())
	{
		Icon = T_Lollipop.Object;
	}
	else
	{
		// COS_SCREEN(TEXT("T_LuchadorMask를 로드할 수 없습니다."));
	}

	Tint = FLinearColor(1.0f,0.47f,0.84f);

	bCanBeZero = true;

	
	
	FToolTipValue ToolTipValue;
	ToolTipValue.bValued=false;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Tooltips_Artifacts(*AssetPath::DataTable::DT_Tooltips_Artifacts);
	if(DT_Tooltips_Artifacts.Succeeded())
	{
		ToolTipValue.ToolTipTable.DataTable = DT_Tooltips_Artifacts.Object;
	}
	else
	{
		COS_SCREEN(TEXT("DT_Tooltips_Artifacts를 로드할 수 없습니다."));
	}
	
	ToolTipValue.ToolTipTable.RowName = TEXT("Lollipop");
	Tooltips.Add(ToolTipValue);

	FriendlyName = FText::FromString(TEXT("Lollipop"));
	
	bShowImmediately = false;
	MaxTriggersPerTick=10;
	CurrentTriggersThisTick=0;
	GameplayTags.AddTag(CosGameTags::Rarity_Invalid);
	bMaxAble=false;
	MaxValue=0;
	bArtifact = true;
	bInterrupt = false;
}




