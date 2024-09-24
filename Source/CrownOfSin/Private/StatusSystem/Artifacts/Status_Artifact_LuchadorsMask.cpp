// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusSystem/Artifacts/Status_Artifact_LuchadorsMask.h"

#include "Core/MinionBase.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/AssetPath.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "StatusSystem/Status_Strength.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


// Sets default values for this component's properties
UStatus_Artifact_LuchadorsMask::UStatus_Artifact_LuchadorsMask()
{
	StatusValue = 0;

	SlotType = EStatusSlot::Artifact;

	static ConstructorHelpers::FObjectFinder<UTexture2D> T_LuchadorMask(*AssetPath::Texture::T_LuchadorMask);
	if(T_LuchadorMask.Succeeded())
	{
		Icon = T_LuchadorMask.Object;
	}
	else
	{
		// COS_SCREEN(TEXT("T_LuchadorMask를 로드할 수 없습니다."));
	}

	Tint = FLinearColor(0.8f,0.16f,0.04f);

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
	
	ToolTipValue.ToolTipTable.RowName = TEXT("LuchadorMask");
	Tooltips.Add(ToolTipValue);

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Tooltips_Statuses(*AssetPath::DataTable::DT_Tooltips_Statuses);
	if(DT_Tooltips_Statuses.Succeeded())
	{
		ToolTipValue.ToolTipTable.DataTable = DT_Tooltips_Statuses.Object;
	}
	else
	{
		COS_SCREEN(TEXT("DT_Tooltips_Statuses를 로드할 수 없습니다."));
	}

	ToolTipValue.ToolTipTable.RowName = TEXT("Strength");
	Tooltips.Add(ToolTipValue);
	
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
void UStatus_Artifact_LuchadorsMask::BeginPlay()
{
	Super::BeginPlay();

	UFunctionLibrary_Event::BindEventToGlobalDispatcherHub(this,CosGameTags::Event_TurnStart);
	
}

void UStatus_Artifact_LuchadorsMask::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject,
	bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	if(EventTag == CosGameTags::Event_TurnStart)
	{
		UFunctionLibrary_Event::UnBindEventFromGlobalDispatcherHub(this,CosGameTags::Event_TurnStart);

		TArray<AActor*> Minions;
		UGameplayStatics::GetAllActorsOfClass(this,AMinionBase::StaticClass(),Minions);

		for (AActor* Minion : Minions)
		{
			AMinionBase* MinionCasted = Cast<AMinionBase>(Minion);

			if (MinionCasted->GetGameplayTags().HasTagExact(CosGameTags::Target_Hero))
			{
				MinionCasted->AddToStatus_Implementation(UStatus_Strength::StaticClass(),StatusValue,true,nullptr);
			}
		}
	}
}




