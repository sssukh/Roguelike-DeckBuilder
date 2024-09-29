// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusSystem/Artifacts/Status_Artifact_CardReward.h"

#include "CardSystem/CardBase.h"
#include "CardSystem/CardEffects/CardEffect_AddNewCardToPile.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Libraries/AssetPath.h"
#include "Libraries/FunctionLibrary_Card.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


// Sets default values for this component's properties
UStatus_Artifact_CardReward::UStatus_Artifact_CardReward()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> CardsRewardTableRef (*AssetPath::DataTable::DT_Cards_Rewards);
	if(CardsRewardTableRef.Succeeded())
	{
		CardsRewardTable = CardsRewardTableRef.Object;
	}
	else
	{
		COS_SCREEN(TEXT("DT_Cards_Rewards를 로드할 수 없습니다."));
	}
	DefaultRewardData.DataTable = CardsRewardTable;
	DefaultRewardData.RowName = TEXT("NormalReward");

	// 설정
	StatusValue = 0;

	SlotType = EStatusSlot::Artifact;

	static ConstructorHelpers::FObjectFinder<UTexture2D> T_CardReward(*AssetPath::Texture::T_CardReward);
	if(T_CardReward.Succeeded())
	{
		Icon = T_CardReward.Object;
	}
	else
	{
		COS_SCREEN(TEXT("T_CardReward를 로드할 수 없습니다."));
	}

	Tint = FLinearColor::White;

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
	
	ToolTipValue.ToolTipTable.RowName = TEXT("CardReward");
	Tooltips.Add(ToolTipValue);

	
	bShowImmediately = false;
	MaxTriggersPerTick=10;
	CurrentTriggersThisTick=0;
	
	bMaxAble=false;
	MaxValue=0;
	bArtifact = true;
	bInterrupt = false;
}

bool UStatus_Artifact_CardReward::Interact_Implementation(const FGameplayTagContainer& Tags)
{
	if(Tags.HasTag(CosGameTags::StatusBar_RewardBar))
	{
		AActor* MainHero = nullptr;

		IInterface_CardGameInstance::Execute_GetMainHero(UFunctionLibrary_Singletons::GetCardGameInstance(this), MainHero);

		ACardBase* NewCard = GetWorld()->SpawnActorDeferred<ACardBase>(ACardBase::StaticClass(),FTransform::Identity,MainHero,nullptr,ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		if(NewCard)
		{
			FDataTableRowHandle CardData;
			CardData.DataTable = CardsRewardTable;
			CardData.RowName = TEXT("GainCard");
			NewCard->CardDataDeck = UFunctionLibrary_Card::MakeCardStructFromCardData(CardData);

			NewCard->FinishSpawning(FTransform::Identity);
		}

		TArray<FCardEffect> CardEffects = NewCard->GetCardEffects(ECardDataType::Hand);

		// 레퍼런스니까 값이 바뀌겠지
		for (FCardEffect& CardEffect : CardEffects)
		{
			if(CardEffect.EffectClass == UCardEffect_AddNewCardToPile::StaticClass())
			{
				CardEffect.UsedData = CardRewardData;
			}
		}

		NewCard->SetCardEffects(ECardDataType::Hand,CardEffects);

		bool result = NewCard->AttemptUseCardUnTargeted(false,false,true);

		RemoveStatus();

		return result;
	}

	// 단순 예외처리
	return false;
}

int32 UStatus_Artifact_CardReward::AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon,
	bool bRefreshAppearance, UObject* InPayload)
{
	int32 ParentValue = Super::AddStatusValue(InAmount, bShowSplashNumber, bShowSplashIcon, bRefreshAppearance, InPayload);

	if(IsValid(CardRewardData.DataTable))
	{
		return ParentValue;
	}

	UCardEffectComponent* CardEffectComponent = Cast<UCardEffectComponent>(InPayload);

	if(!CardEffectComponent)
	{
		return ParentValue;
	}

	if(IsValid(CardEffectComponent->UsedData.DataTable))
	{
		CardRewardData = CardEffectComponent->UsedData;
	}
	else
	{
		CardRewardData = DefaultRewardData;
	}

	return ParentValue;
}




