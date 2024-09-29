// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusSystem/Artifacts/Status_Artifact_RobesOfProtection.h"

#include "CardSystem/CardPlayer.h"
#include "CardSystem/Piles/PileDrawComponent.h"
#include "Core/DispatcherHubComponent.h"
#include "Core/MinionBase.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/AssetPath.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "StatusSystem/Status_Armor.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


// Sets default values for this component's properties
UStatus_Artifact_RobesOfProtection::UStatus_Artifact_RobesOfProtection()
{
	StatusValue = 0;

	SlotType = EStatusSlot::Artifact;

	static ConstructorHelpers::FObjectFinder<UTexture2D> T_RobesOfProtection(*AssetPath::Texture::T_RobesOfProtection);
	if(T_RobesOfProtection.Succeeded())
	{
		Icon = T_RobesOfProtection.Object;
	}
	else
	{
		COS_SCREEN(TEXT("T_RobesOfProtection을 로드할 수 없습니다."));
	}

	Tint = FLinearColor::White;

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
	
	ToolTipValue.ToolTipTable.RowName = TEXT("RobesOfProtection");
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

	ToolTipValue2.ToolTipTable.RowName = TEXT("Armor");
	Tooltips.Add(ToolTipValue2);

	FriendlyName =FText::FromString("Robes Of Protection");
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
void UStatus_Artifact_RobesOfProtection::BeginPlay()
{
	Super::BeginPlay();

	UDispatcherHubComponent* DispatcherHubComponent = nullptr;
	if(GetOwnersDispatcherHub(DispatcherHubComponent))
	{
		DispatcherHubComponent->BindEventToHub(this,CosGameTags::Event_TurnStart);
	}
	
}

void UStatus_Artifact_RobesOfProtection::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject,
	bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	if(EventTag != CosGameTags::Event_TurnStart)
	{
		return;
	}

	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

	UPileDrawComponent* PileDrawComponent = CardPlayer->FindComponentByClass<UPileDrawComponent>();

	if(PileDrawComponent&&PileDrawComponent->Cards.Num()>=EffectCutoff)
	{
		TArray<AActor*> Minions;
		UGameplayStatics::GetAllActorsOfClass(this,AMinionBase::StaticClass(),Minions);

		for (AActor* Minion : Minions)
		{
			if(Cast<AMinionBase>(Minion)->GetGameplayTags().HasTagExact(CosGameTags::Target_Hero))
			{
				IInterface_CardTarget::Execute_AddToStatus(Minion,UStatus_Armor::StaticClass(),StatusValue,true,nullptr);
			}
		}
	}
}



