// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusSystem/Artifacts/Status_Artifact_BagOfHolding.h"

#include "Interfaces/Interface_CardTarget.h"
#include "Libraries/AssetPath.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "StatusSystem/Status_Mana.h"
#include "StatusSystem/Status_ManaGain.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


// Sets default values for this component's properties
UStatus_Artifact_BagOfHolding::UStatus_Artifact_BagOfHolding()
{
	StatusValue = 0;

	SlotType = EStatusSlot::Artifact;

	static ConstructorHelpers::FObjectFinder<UTexture2D> T_BagOfHolding(*AssetPath::Texture::T_BagOfHolding);
	if(T_BagOfHolding.Succeeded())
	{
		Icon = T_BagOfHolding.Object;
	}
	else
	{
		COS_SCREEN(TEXT("T_BagOfHolding을 로드할 수 없습니다."));
	}

	Tint = FLinearColor(0.8f,0.38f,0.14f);

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
	
	ToolTipValue.ToolTipTable.RowName = TEXT("BagOfHolding");
	Tooltips.Add(ToolTipValue);

	FriendlyName = FText::FromString(TEXT("Bag Of Holding"));
	
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
void UStatus_Artifact_BagOfHolding::BeginPlay()
{
	Super::BeginPlay();

	UFunctionLibrary_Event::BindEventToGlobalDispatcherHub(this,CosGameTags::Event_ManaGain);
	
}

void UStatus_Artifact_BagOfHolding::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject,
	bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	if(EventTag == CosGameTags::Event_ManaGain)
	{
		UStatus_ManaGain* ManaGain = Cast<UStatus_ManaGain>(CallingObject);

		if(!ManaGain)
			return;

		ManaGain->Interrupt();

		if(GetOwner()->GetClass()->ImplementsInterface(UInterface_CardTarget::StaticClass()))
		{
			IInterface_CardTarget::Execute_AddToStatus(GetOwner(),UStatus_Mana::StaticClass(),ManaGain->StatusValue,false,nullptr);
		}
	}
}




