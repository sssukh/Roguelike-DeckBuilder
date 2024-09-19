// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/TargetSystem/TargetingComponent_DiscoverArtifact.h"

#include "CardSystem/CardPlayer.h"
#include "CardSystem/ChanceManagerComponent.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/AssetPath.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "Rendering/StaticLightingSystemInterface.h"
#include "StatusSystem/StatusComponent.h"
#include "UI/UW_ArtifactRewardScreen.h"
#include "UI/UW_Layout_Cos.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


// Sets default values for this component's properties
UTargetingComponent_DiscoverArtifact::UTargetingComponent_DiscoverArtifact()
{
	PickedArtifact.Value = 1;

	CardRewardScreen = nullptr;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_RarityWeights(*AssetPath::DataTable::DT_RarityWeights);
	if (DT_RarityWeights.Succeeded())
	{
		RarityWeightsTable = DT_RarityWeights.Object;
	}
	else
	{
		COS_LOG_ERROR(TEXT("를 로드하지 못했습니다."));
	}
}

bool UTargetingComponent_DiscoverArtifact::FindValidTargets(TArray<AActor*>& SpecifiedTargets,
                                                            const FCardEffect& CardEffect, ACardBase* Card, bool bPreview, TArray<AActor*>& OutValidTargets)
{
	if (bPreview)
		return false;

	FDataTableRowHandle CardEffectUsedData = CardEffect.UsedData;

	FRarityWeights* RarityWeightsFound = CardEffectUsedData.DataTable->FindRow<FRarityWeights>(CardEffectUsedData.RowName,TEXT("FRarityWeights in TargetingComponent_DiscoverArtifact"));

	// 받아온 데이터가 유효하지 않으면
	if (CardEffectUsedData.IsNull() || !RarityWeightsFound)
	{
		RarityWeightsFound = RarityWeightsTable->FindRow<FRarityWeights>(TEXT("Normal"),TEXT("FRarityWeights in TargetingComponent_DiscoverArtifact"));
		if (!RarityWeightsFound)
			return false;
	}

	TMap<FGameplayTag, float> RarityWeights = RarityWeightsFound->RarityWeights;


	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

	TArray<FStatusData> PickedArtifacts;

	if (!CardPlayer->ChanceManagerComponent->GetRandomArtifactsByCustomTagWeights(RarityWeights, 1, true, PickedArtifacts))
	{
		return false;
	}

	PickedArtifact = PickedArtifacts[0];

	TempArtifactHolder = GetWorld()->SpawnActor(AActor::StaticClass());

	UStatusComponent* StatusComponent = NewObject<UStatusComponent>(this);

	if (StatusComponent)
	{
		StatusComponent->StatusValue = PickedArtifact.Value;
		StatusComponent->GameplayTags = PickedArtifact.GameplayTags;
		StatusComponent->bShowImmediately = true;

		StatusComponent->RegisterComponent();
	}

	// TODO : ActionArtifactRewardScreen 을 생성해서 Artifact에 값 넣어주기
	// GetWorld()->SpawnActorDeferred<ActionArti>()


	BindToArtifactConfirm(CardPlayer->PlayerUI->WBP_ArtifactReward);

	return true;
}

void UTargetingComponent_DiscoverArtifact::BindToArtifactConfirm(UUW_ArtifactRewardScreen* InCardRewardScreen)
{
	CardRewardScreen = InCardRewardScreen;
	InCardRewardScreen->OnReturnSelectionArtifact.AddDynamic(this, &UTargetingComponent_DiscoverArtifact::ValidateAndTransferArtifact);
}

void UTargetingComponent_DiscoverArtifact::ValidateAndTransferArtifact(bool bSkipped, UStatusComponent* Artifact)
{
	CardRewardScreen->OnReturnSelectionArtifact.RemoveDynamic(this, &UTargetingComponent_DiscoverArtifact::ValidateAndTransferArtifact);

	if (!bSkipped)
	{
		ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

		CardPlayer->AddToStatus(Artifact->GetClass(), Artifact->StatusValue, true, nullptr);
	}

	TempArtifactHolder->Destroy();

	TArray<AActor*> Targets;

	OnInputTargetsReceived.Broadcast(Targets);

	if (UGameplayStatics::GetGameInstance(this)->GetClass()->ImplementsInterface(UInterface_CardGameInstance::StaticClass()))
	{
		IInterface_CardGameInstance::Execute_AttemptSaveGame(UGameplayStatics::GetGameInstance(this),TEXT(""), false);
	}
}
