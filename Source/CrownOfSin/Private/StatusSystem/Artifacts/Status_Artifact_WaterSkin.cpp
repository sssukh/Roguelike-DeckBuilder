// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusSystem/Artifacts/Status_Artifact_WaterSkin.h"

#include "CardSystem/CardPlayer.h"
#include "Libraries/AssetPath.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "StatusSystem/Status_Mana.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


// Sets default values for this component's properties
UStatus_Artifact_WaterSkin::UStatus_Artifact_WaterSkin()
{
	DoOnce = FDoOnce(false);
	
	StatusValue = 0;

	SlotType = EStatusSlot::Artifact;

	static ConstructorHelpers::FObjectFinder<UTexture2D> T_WaterSkin(*AssetPath::Texture::T_WaterSkin);
	if(T_WaterSkin.Succeeded())
	{
		Icon = T_WaterSkin.Object;
	}
	else
	{
		COS_SCREEN(TEXT("T_WaterSkin을 로드할 수 없습니다."));
	}

	Tint = FLinearColor(0.8f,0.63f,0.31f);

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
	
	ToolTipValue.ToolTipTable.RowName = TEXT("WaterSkin");
	Tooltips.Add(ToolTipValue);
	
	FToolTipValue ToolTipValue2;
	ToolTipValue2.bValued=false;
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Tooltips_Statuses(*AssetPath::DataTable::DT_Tooltips_Statuses);
	if(DT_Tooltips_Statuses.Succeeded())
	{
		ToolTipValue2.ToolTipTable.DataTable = DT_Tooltips_Statuses.Object;
	}
	else
	{
		COS_SCREEN(TEXT("DT_Tooltips_Statuses를 로드할 수 없습니다."));
	}

	ToolTipValue2.ToolTipTable.RowName = TEXT("Exhaust");
	Tooltips.Add(ToolTipValue2);
	
	
	bShowImmediately = false;
	MaxTriggersPerTick=10;
	CurrentTriggersThisTick=0;
	GameplayTags.AddTag(CosGameTags::Rarity_Common);
	bMaxAble=false;
	MaxValue=0;
	bArtifact = true;
	bInterrupt = false;
}


// Called when the game starts
void UStatus_Artifact_WaterSkin::BeginPlay()
{
	Super::BeginPlay();

	UFunctionLibrary_Event::BindEventToGlobalDispatcherHub(this,CosGameTags::Event_Card_Exhaust);

	UFunctionLibrary_Event::BindEventToGlobalDispatcherHub(this,CosGameTags::Event_TurnStart);
	
}

void UStatus_Artifact_WaterSkin::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject,
	bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	if(EventTag == CosGameTags::Event_Card_Exhaust)
	{
		if(DoOnce.Execute())
		{
			ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

			if(UStatus_Mana* Status_Mana = CardPlayer->FindComponentByClass<UStatus_Mana>())
			{
				Status_Mana->AddStatusValue(StatusValue,false,false,false,nullptr);
			}
		}
	}
	else if(EventTag == CosGameTags::Event_TurnStart)
	{
		DoOnce.Reset();
	}
}




