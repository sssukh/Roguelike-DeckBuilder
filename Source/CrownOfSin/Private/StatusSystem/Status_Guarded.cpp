// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusSystem/Status_Guarded.h"

#include "CardSystem/CardEffects/CardEffectComponent.h"
#include "Core/DispatcherHubComponent.h"
#include "Core/MinionBase.h"
#include "Core/MinionTrack.h"
#include "Libraries/AssetPath.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


UStatus_Guarded::UStatus_Guarded()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	static ConstructorHelpers::FObjectFinder<UTexture2D> T_King(*AssetPath::Texture::T_King);
	if (T_King.Succeeded())
	{
		Icon = T_King.Object;
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("T_King을 로드하지 못했습니다."))
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Tooltips_Statuses(*AssetPath::DataTable::DT_Tooltips_Statuses);
	if (DT_Tooltips_Statuses.Succeeded())
	{
		FToolTipValue NewToolTipValue;
		NewToolTipValue.ToolTipTable.DataTable = DT_Tooltips_Statuses.Object;
		NewToolTipValue.ToolTipTable.RowName = FName(TEXT("Guarded"));
		NewToolTipValue.bValued = false;
		Tooltips.Add(NewToolTipValue);
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("DT_Tooltips_Statuses를 로드하지 못했습니다"));
	}

	Tint = FLinearColor::Yellow;
	GameplayTags = FGameplayTagContainer(CosGameTags::Effect_Buff);
}

void UStatus_Guarded::BeginPlay()
{
	Super::BeginPlay();

	// ...

	if (AMinionBase* CastOwner = Cast<AMinionBase>(GetOwner()))
	{
		OwningMinion = CastOwner;

		UDispatcherHubComponent* DispatcherHub;
		if (GetOwnersDispatcherHub(DispatcherHub))
		{
			DispatcherHub->BindEventToHub(this, CosGameTags::Event_IncomingAttack);
		}
	}
}

void UStatus_Guarded::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	if (EventTag == CosGameTags::Event_IncomingAttack)
	{
		if (OwningMinion->MinionTrack->Minions.Num() > 1)
		{
			if (UCardEffectComponent* CallingCardEffect = Cast<UCardEffectComponent>(CallingObject))
			{
				CallingCardEffect->EffectValue = CallingCardEffect->EffectValue * 0.5f;
			}
		}
	}
}
