#include "StatusSystem/Artifacts/Status_Artifact_CardReward.h"

#include "CardSystem/CardBase.h"
#include "CardSystem/CardEffects/CardEffect_AddNewCardToPile.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Libraries/AssetPath.h"
#include "Libraries/FunctionLibrary_Card.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


UStatus_Artifact_CardReward::UStatus_Artifact_CardReward()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	static ConstructorHelpers::FObjectFinder<UTexture2D> T_CardReward(*AssetPath::Texture::T_CardReward);
	if (T_CardReward.Succeeded())
	{
		Icon = T_CardReward.Object;
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("T_CardReward를 로드하지 못했습니다."))
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Tooltips_Artifacts(*AssetPath::DataTable::DT_Tooltips_Artifacts);
	if (DT_Tooltips_Artifacts.Succeeded())
	{
		FToolTipValue NewToolTipValue;
		NewToolTipValue.ToolTipTable.DataTable = DT_Tooltips_Artifacts.Object;
		NewToolTipValue.ToolTipTable.RowName = FName(TEXT("CardReward"));
		NewToolTipValue.bValued = false;
		Tooltips.Add(NewToolTipValue);
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("DT_Tooltips_Artifacts 로드하지 못했습니다"));
	}


	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Cards_Rewards(*AssetPath::DataTable::DT_Cards_Rewards);
	if (DT_Cards_Rewards.Succeeded())
	{
		DefaultRewardData.DataTable = DT_Cards_Rewards.Object;
		DefaultRewardData.RowName = FName(TEXT("NormalReward"));
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("DT_Cards_Rewards 로드하지 못했습니다"));
	}
}


void UStatus_Artifact_CardReward::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

int32 UStatus_Artifact_CardReward::AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, bool bRefreshAppearance, UObject* InPayload)
{
	int32 AddStatusValue = Super::AddStatusValue(InAmount, bShowSplashNumber, bShowSplashIcon, bRefreshAppearance, InPayload);

	if (!IsValid(CardRewardData.DataTable))
	{
		if (UCardEffectComponent* PayLoadCardEffectComponent = Cast<UCardEffectComponent>(InPayload))
		{
			CardRewardData = IsValid(PayLoadCardEffectComponent->UsedData.DataTable) ? PayLoadCardEffectComponent->UsedData : DefaultRewardData;
		}
	}

	return AddStatusValue;
}

bool UStatus_Artifact_CardReward::Interact_Implementation(const FGameplayTagContainer& Tags)
 {
	// 'StatusBar_RewardBar' 태그를 가진 경우에만 실행
	if (!Tags.HasTagExact(CosGameTags::StatusBar_RewardBar))
		return false;

	// 보상 데이터가 유효한지 확인
	COS_IF_CHECK_RETURN(!DefaultRewardData.IsNull(), TEXT("DefaultRewardData를 설정해주세요"), false);

	// 상호작용 태그 배열을 순회하면서 'StatusBar_RewardBar' 태그와 일치하는지 확인
	for (const FGameplayTag& Tag : Tags.GetGameplayTagArray())
	{
		// 'StatusBar_RewardBar'가 아닌 태그는 무시
		if (Tag != CosGameTags::StatusBar_RewardBar)
			continue;

		// 메인 캐릭터를 가져옴
		UGameInstance* GameInstance = UFunctionLibrary_Singletons::GetCardGameInstance(this);
		AActor* MainHero = nullptr;
		IInterface_CardGameInstance::Execute_GetMainHero(GameInstance, MainHero);

		// 메인 캐릭터가 없는 경우 실패
		if (!MainHero)
			return false;

		// 카드를 스폰할 위치와 기본 설정
		FTransform SpawnTransform = FTransform::Identity;
		ACardBase* SpawnCard = GetWorld()->SpawnActorDeferred<ACardBase>(ACardBase::StaticClass(), SpawnTransform,
		                                                                 MainHero, nullptr,
		                                                                 ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
		                                                                 ESpawnActorScaleMethod::OverrideRootScale);


		FDataTableRowHandle NewCardDataTableRowHandle = DefaultRewardData;
		NewCardDataTableRowHandle.RowName = FName("GainCard");

		FCard NewCardData = UFunctionLibrary_Card::MakeCardStructFromCardData(NewCardDataTableRowHandle);
		SpawnCard->CardDataDeck = NewCardData;

		// 카드 스폰 완료
		SpawnCard->FinishSpawning(SpawnTransform);

		// 스폰된 카드의 효과 목록 가져옴
		TArray<FCardEffect> LocalCardEffects = SpawnCard->GetCardEffects(ECardDataType::Hand);

		// 카드 효과에 추가 카드 보상 적용
		for (FCardEffect& CardEffect : LocalCardEffects)
		{
			if (CardEffect.EffectClass->IsChildOf(UCardEffect_AddNewCardToPile::StaticClass()))
			{
				CardEffect.UsedData = CardRewardData;
			}
		}

		// 적용된 카드 효과를 업데이트
		SpawnCard->SetCardEffects(ECardDataType::Hand, LocalCardEffects);

		// 카드를 사용하고 성공 여부를 반환
		bool bSuccess = SpawnCard->AttemptUseCardUnTargeted(false, false, true);

		// 상태 제거 후 성공 여부 반환
		RemoveStatus();
		return bSuccess;
	}

	return false;
}
