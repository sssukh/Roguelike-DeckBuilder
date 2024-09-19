#include "CardSystem/ChanceManagerComponent.h"

#include "BlueprintGameplayTagLibrary.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Libraries/AssetPath.h"
#include "Libraries/FunctionLibrary_ArrayUtils.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "StatusSystem/StatusComponent.h"
#include "StatusSystem/Artifacts/Status_Artifact_Lollipop.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


UChanceManagerComponent::UChanceManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Cards_Rewards(*AssetPath::DataTable::DT_Cards_Rewards);
	if (DT_Cards_Rewards.Succeeded())
	{
		CardRewardDataRowHandle.DataTable = DT_Cards_Rewards.Object;
		CardRewardDataRowHandle.RowName = FName(TEXT("ConsolationPrize"));
	}
	else
	{
		COS_LOG_ERROR(TEXT("DT_Cards_Rewards를 로드하지 못했습니다."));
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Artifacts(*AssetPath::DataTable::DT_Artifacts);
	if (DT_Artifacts.Succeeded())
	{
		ArtifactTables.Add(DT_Artifacts.Object);
	}
	else
	{
		COS_LOG_ERROR(TEXT("DT_Artifacts 를 로드하지 못했습니다."));
	}
	

	DefaultArtifactRarityWeights.Add(CosGameTags::Rarity_Common, 1.0f);
	DefaultArtifactRarityWeights.Add(CosGameTags::Rarity_Rare, 0.0f);
	DefaultArtifactRarityWeights.Add(CosGameTags::Rarity_Epic, 0.0f);
	DefaultArtifactRarityWeights.Add(CosGameTags::Rarity_Legendary, 0.0f);
}

void UChanceManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	GenerateAllCardsArray();

	GenerateAllArtifactsArray();
}

TArray<FCard> UChanceManagerComponent::GenerateAllCardsArray()
{
	AllCards.Empty();
	UGameInstance* CardGameInstance = UFunctionLibrary_Singletons::GetCardGameInstance(this);
	if (!CardGameInstance) return AllCards;

	const TArray<UDataTable*>& RewardTables = IInterface_CardGameInstance::Execute_GetRewardTables(CardGameInstance);
	for (UDataTable* RewardTable : RewardTables)
	{
		const TArray<FName>& RewardNames = RewardTable->GetRowNames();
		for (const FName& RewardName : RewardNames)
		{
			if (FCard* FoundRewardCard = RewardTable->FindRow<FCard>(RewardName,TEXT("")))
			{
				FCard NewCard = *FoundRewardCard;
				NewCard.DataRow.DataTable = RewardTable;
				NewCard.DataRow.RowName = RewardName;
				AllCards.Add(NewCard);
			}
		}
	}
	return AllCards;
}

TArray<FStatusData> UChanceManagerComponent::GenerateAllArtifactsArray()
{
	for (UDataTable* ArtifactTable : ArtifactTables)
	{
		const TArray<FName>& ArtifactRowNames = ArtifactTable->GetRowNames();
		for (const FName& ArtifactRowName : ArtifactRowNames)
		{
			if (FStatusData* FoundArtifact = ArtifactTable->FindRow<FStatusData>(ArtifactRowName,TEXT("")))
			{
				AllArtifacts.Add(*FoundArtifact);
			}
		}
	}

	return AllArtifacts;
}

TArray<FGameplayTag> UChanceManagerComponent::GetRandomTagsByWeights(const TMap<FGameplayTag, float>& WeightedTags, int InAmount)
{
	// 가중치 값을 저장할 배열 생성
	TArray<float> WeightValues;
	WeightedTags.GenerateValueArray(WeightValues);

	// 최소 및 최대 가중치 계산
	float MinWeight = FMath::Min(WeightValues);
	float MaxWeight = FMath::Max(WeightValues);

	// 선택된 태그를 저장할 배열
	TArray<FGameplayTag> PickedTags;

	// 모든 가중치가 동일한 경우 랜덤으로 태그 선택
	if (MinWeight == MaxWeight)
	{
		// 가중치가 없는 상태에서 태그 선택
		PickedTags = GetRandomTagsWithoutWeight(WeightedTags, InAmount);
	}
	else
	{
		// 가중치 총합을 계산하고, 가중치에 따라 태그 선택
		float TotalWeight = GetTotalWeight(WeightValues);
		PickedTags = GetRandomTagsWithWeight(WeightedTags, InAmount, TotalWeight);
	}

	// 선택된 태그 배열 반환
	return PickedTags;
}

bool UChanceManagerComponent::PickTagFromWeightAndWeightSum(TMap<FGameplayTag, float> WeightedTags, float TotalWeight, FGameplayTag& OutPickedTag)
{
	// 기준값: 0과 가중치 총합 사이에서 랜덤 값 선택
	float RandomWeight = FMath::FRandRange(0.0f, TotalWeight);

	// 누적 가중치
	float AccumulatedWeight = 0.0f;

	// 가중치 맵을 순회하여 태그 선택
	for (const TPair<FGameplayTag, float>& TagPair : WeightedTags)
	{
		AccumulatedWeight += TagPair.Value;

		// 누적된 가중치가 랜덤 값에 도달하면 해당 태그를 선택
		if (AccumulatedWeight >= RandomWeight)
		{
			OutPickedTag = TagPair.Key;
			return true;
		}
	}

	// 만약 태그를 선택하지 못한 경우 (논리적으로는 실행되지 않음)
	return false;
}

bool UChanceManagerComponent::FindPickedTagAmongFilteredCards(FGameplayTag PickedCardTag, TArray<FCard>& FilteredCards, bool bPreventDuplicates, FCard& OutPickedCard)
{
	// 필터링된 카드 목록을 뒤에서부터 순회
	for (int i = FilteredCards.Num() - 1; i >= 0; --i)
	{
		// 현재 카드가 선택된 태그를 가지고 있는지 확인
		if (FilteredCards[i].CardTags.HasTagExact(PickedCardTag))
		{
			// 중복 방지를 활성화한 경우, 카드를 목록에서 제거
			if (bPreventDuplicates)
			{
				FilteredCards.RemoveAt(i);
			}

			OutPickedCard = FilteredCards[i]; // 찾은 카드를 출력 변수에 할당
			return true; // 성공적으로 카드를 찾았음을 반환
		}
	}

	// 해당 태그를 가진 카드를 찾지 못한 경우 false 반환
	return false;
}

bool UChanceManagerComponent::FindPickedTagAmongFilteredArtifacts(FGameplayTag PickedTag, TArray<FStatusData>& FilteredArtifacts, bool bPreventDuplicates, FStatusData& OutPickedArtifact)
{
	// 인덱스를 초기화
	int32 Index = 0;

	// 필터링된 아티팩트에서 선택된 태그를 찾음
	for (const FStatusData& FilteredArtifact : FilteredArtifacts)
	{
		// 태그가 정확히 일치하는 아티팩트를 찾음
		if (FilteredArtifact.GameplayTags.HasTagExact(PickedTag))
		{
			// 중복 방지가 설정된 경우, 해당 아티팩트를 리스트에서 제거
			if (bPreventDuplicates)
			{
				FilteredArtifacts.RemoveAt(Index);
			}

			// 찾은 아티팩트를 반환
			OutPickedArtifact = FilteredArtifact;
			return true;
		}

		// 다음 인덱스로 이동
		Index++;
	}

	// 태그와 일치하는 아티팩트를 찾지 못한 경우 기본값 반환
	OutPickedArtifact = FStatusData();
	return false;
}

TArray<FCard> UChanceManagerComponent::GetFilteredCardsWithAllTags(FGameplayTagContainer Filter, const TArray<FCard>& Cards, bool bExactMatch)
{
	// 필터링된 카드를 저장할 배열
	TArray<FCard> FilteredCards;

	// 카드 배열을 순회하면서 필터 조건을 충족하는 카드를 필터링
	for (const FCard& Card : Cards)
	{
		// 카드의 태그가 모두 필터 조건을 만족하는 경우
		if (UBlueprintGameplayTagLibrary::HasAllTags(Card.CardTags, Filter, bExactMatch))
		{
			FilteredCards.Add(Card);
		}
	}

	// 필터링된 카드 배열 반환
	return FilteredCards;
}

TArray<FCard> UChanceManagerComponent::GetFilteredCardsWithAnyTag(FGameplayTagContainer Filter, const TArray<FCard>& Cards, bool bExactMatch)
{
	// 필터링된 카드를 저장할 배열
	TArray<FCard> FilteredCards;

	// 카드 배열을 순회하면서 필터 조건을 충족하는 카드를 필터링
	for (const FCard& Card : Cards)
	{
		// 카드가 필터 조건 중 하나라도 만족하는 경우
		if (UBlueprintGameplayTagLibrary::HasAnyTags(Card.CardTags, Filter, bExactMatch))
		{
			FilteredCards.Add(Card); // 해당 카드를 필터링된 카드 배열에 추가
		}
	}

	// 필터링된 카드 배열 반환
	return FilteredCards;
}

TArray<FCard> UChanceManagerComponent::GetRandomCardsByTagWeights(TMap<FGameplayTag, float> TagWeights, FGameplayTagContainer PossibleTags, FGameplayTagContainer RequiredTags,
                                                                  bool bExactMatch, int32 InAmount)
{
	// 전체 카드 목록에서 필터링된 카드 목록 생성
	TArray<FCard> FilteredCards = AllCards;

	// 필수 태그로 카드 필터링
	FilterCardsByRequiredTags(RequiredTags, bExactMatch, FilteredCards);

	// 가능한 태그로 추가 필터링
	FilterCardsByPossibleTags(PossibleTags, bExactMatch, FilteredCards);

	// 필터링된 카드가 존재하는 경우
	if (FilteredCards.Num() > 0)
	{
		// 태그 가중치가 있는 경우 가중치를 적용하여 카드 선택
		if (TagWeights.Num() > 0)
		{
			return ProcessCardsWithWeights(FilteredCards, TagWeights, InAmount);
		}

		// 가중치가 없는 경우 무작위로 카드를 선택
		return PickRandomCardsWithoutWeights(FilteredCards, InAmount);
	}

	// 필터링된 카드가 없을 경우 보상 카드 처리
	return HandleNoFilteredCards(InAmount);
}

TArray<FCard> UChanceManagerComponent::GetRandomFilteredCards(int32 InAmount, FGameplayTagContainer PossibleTags, FGameplayTagContainer RequiredTags, bool bExactMatch)
{
	// 빈 가중치 맵을 정의하여 가중치 없이 무작위 선택을 처리
	TMap<FGameplayTag, float> NoWeights;

	// 태그 가중치 없이 필터링된 카드들 중에서 무작위로 InAmount만큼 카드를 선택하여 반환
	return GetRandomCardsByTagWeights(NoWeights, PossibleTags, RequiredTags, bExactMatch, InAmount);
}

TArray<FCard> UChanceManagerComponent::GetTrueRandomCardsOfAnyRarity(int32 InAmount)
{
	// 반환할 카드 리스트 초기화
	TArray<FCard> PickedCards;

	// 희귀도 관련 태그를 필터링에 사용
	FGameplayTag RarityTag = CosGameTags::Rarity;

	// 주어진 수량만큼 무작위로 필터링된 카드를 가져옴 (희귀도 태그로 필터링)
	TArray<FCard> RandomCards = GetRandomFilteredCards(InAmount, FGameplayTagContainer(), FGameplayTagContainer(RarityTag), false);

	// 선택할 카드 수가 0 이하인 경우 기본 카드 한 장을 반환
	if (InAmount <= 0)
	{
		if (RandomCards.Num() > 0)
		{
			PickedCards.Add(RandomCards[0]); // 첫 번째 카드를 기본으로 선택
		}
		return PickedCards;
	}

	// 요청된 카드 수만큼 무작위 카드 선택
	for (int32 i = 0; i < InAmount; ++i)
	{
		// 인덱스 초과를 방지하기 위해 RandomCards의 크기에 맞춰 모듈로 연산 사용
		int32 Index = i % RandomCards.Num();
		PickedCards.Add(RandomCards[Index]);
	}

	return PickedCards;
}

TArray<FStatusData> UChanceManagerComponent::GetRemainingArtifacts()
{
	// '남은 아티팩트' 리스트를 '모든 아티팩트'로 초기화합니다.
	TArray<FStatusData> RemainingArtifacts = AllArtifacts;

	// 현재 게임 인스턴스를 가져옵니다.
	UGameInstance* CardGameInstance = UFunctionLibrary_Singletons::GetCardGameInstance(this);
	if (!CardGameInstance)
	{
		// 게임 인스턴스를 찾을 수 없으면 모든 아티팩트를 그대로 반환합니다.
		return RemainingArtifacts;
	}

	// 게임 인스턴스에서 이미 획득한 아티팩트 리스트를 가져옵니다.
	const TArray<FStatusData>& AcquiredArtifacts = IInterface_CardGameInstance::Execute_GetArtifactsFromInstance(CardGameInstance);

	// 이미 획득한 아티팩트를 '모든 아티팩트' 리스트에서 제거합니다.
	for (const FStatusData& AcquiredArtifact : AcquiredArtifacts)
	{
		for (const FStatusData& Artifact : AllArtifacts)
		{
			// 아티팩트 클래스가 동일한 경우 해당 아티팩트를 남은 아티팩트 리스트에서 제거합니다.
			if (Artifact.StatusClass == AcquiredArtifact.StatusClass)
			{
				RemainingArtifacts.Remove(Artifact);
			}
		}
	}

	// 남은 아티팩트 리스트를 반환합니다.
	return RemainingArtifacts;
}

bool UChanceManagerComponent::GetRandomArtifactsByCustomTagWeights(TMap<FGameplayTag, float> TagWeights, int32 InAmount, bool bExactMatch, TArray<FStatusData>& OutPickedArtifacts)
{
	// 남아있는 아티팩트를 가져옴
	TArray<FStatusData> RemainingArtifacts = GetRemainingArtifacts();

	// 아티팩트 리스트를 무작위로 섞음
	UFunctionLibrary_ArrayUtils::ShuffleArray(RemainingArtifacts);

	// 주어진 가중치 맵을 사용하여 선택된 태그들을 가져옴
	TArray<FGameplayTag> PickedTags = GetRandomTagsByWeights(TagWeights, InAmount);

	// 선택된 아티팩트 리스트를 저장할 배열 초기화
	TArray<FStatusData> PickedArtifacts;

	// 각 선택된 태그에 대해 아티팩트를 찾아서 선택
	for (FGameplayTag PickedTag : PickedTags)
	{
		FStatusData PickedArtifact;

		// 필터링된 아티팩트에서 태그가 일치하는 아티팩트를 찾아서 추가
		if (FindPickedTagAmongFilteredArtifacts(PickedTag, RemainingArtifacts, bExactMatch, PickedArtifact))
		{
			PickedArtifacts.Add(PickedArtifact);
		}
	}

	// 만약 선택된 아티팩트 수가 원하는 수량보다 적다면, 기본 아티팩트를 채워 넣음
	FStatusData FillerArtifact;
	FillerArtifact.Value = 1;
	FillerArtifact.StatusClass = UStatus_Artifact_Lollipop::StaticClass();

	// 부족한 아티팩트 수만큼 기본 아티팩트를 추가
	while (PickedArtifacts.Num() < InAmount)
	{
		PickedArtifacts.Add(FillerArtifact);
	}

	// 결과를 출력 배열에 저장
	OutPickedArtifacts = PickedArtifacts;

	return true;
}

bool UChanceManagerComponent::GetRandomArtifactsByDefaultRarityWeights(int32 InAmount, TArray<FStatusData>& OutPickedArtifacts)
{
	return GetRandomArtifactsByCustomTagWeights(DefaultArtifactRarityWeights, InAmount, true, OutPickedArtifacts);
}

TArray<FGameplayTag> UChanceManagerComponent::GetRandomTagsWithoutWeight(const TMap<FGameplayTag, float>& WeightedTags, int32 InAmount)
{
	TArray<FGameplayTag> RandomTags;
	TArray<FGameplayTag> Keys;

	WeightedTags.GetKeys(Keys);
	UFunctionLibrary_ArrayUtils::ShuffleArray(Keys);

	// 태그를 임의로 선택하여 결과에 추가
	while (RandomTags.Num() < InAmount)
	{
		RandomTags.Add(Keys[FMath::RandRange(0, Keys.Num() - 1)]);
	}

	return RandomTags;
}

float UChanceManagerComponent::GetTotalWeight(const TArray<float>& WeightValues)
{
	float TotalWeight = 0.0f;
	for (float Weight : WeightValues)
	{
		TotalWeight += Weight;
	}
	return TotalWeight;
}

TArray<FGameplayTag> UChanceManagerComponent::GetRandomTagsWithWeight(const TMap<FGameplayTag, float>& WeightedTags, int32 InAmount, float TotalWeight)
{
	// 선택된 태그를 저장할 배열
	TArray<FGameplayTag> WeightedTagsResult;

	// 선택할 태그의 수만큼 반복
	while (WeightedTagsResult.Num() < InAmount)
	{
		FGameplayTag PickedTag;

		// 가중치와 총합을 기반으로 태그를 무작위로 선택
		if (PickTagFromWeightAndWeightSum(WeightedTags, TotalWeight, PickedTag))
		{
			// 선택된 태그를 결과 배열에 추가
			WeightedTagsResult.Add(PickedTag);
		}
	}

	// 선택된 태그 배열 반환
	return WeightedTagsResult;
}

void UChanceManagerComponent::FilterCardsByRequiredTags(const FGameplayTagContainer& RequiredTags, bool bExactMatch, TArray<FCard>& OutCards)
{
	// 필수 태그가 있는 경우 필터링을 수행
	if (RequiredTags.Num() > 0)
	{
		// 필수 태그를 모두 가지고 있는 카드만 필터링하여 반환
		OutCards = GetFilteredCardsWithAllTags(RequiredTags, OutCards, bExactMatch);
	}
}

void UChanceManagerComponent::FilterCardsByPossibleTags(const FGameplayTagContainer& PossibleTags, bool bExactMatch, TArray<FCard>& OutCards)
{
	// 가능한 태그가 있는 경우 필터링을 수행
	if (PossibleTags.Num() > 0)
	{
		// 가능한 태그 중 하나라도 가지고 있는 카드를 필터링하여 반환
		OutCards = GetFilteredCardsWithAnyTag(PossibleTags, OutCards, bExactMatch);
	}
}

TArray<FCard> UChanceManagerComponent::ProcessCardsWithWeights(TArray<FCard>& FilteredCards, const TMap<FGameplayTag, float>& TagWeights, int32 InAmount)
{
	// 필터링된 카드에 태그 가중치 적용
	ApplyTagWeightsToFilteredCards(FilteredCards, TagWeights);

	// 태그 가중치 기반으로 무작위로 태그를 선택
	TArray<FGameplayTag> SelectedTags = GetRandomTagsByWeights(TagWeights, InAmount);

	// 선택된 태그에 해당하는 카드를 선택
	TArray<FCard> SelectedCards = PickCardsByTags(FilteredCards, SelectedTags);

	// 선택된 카드가 있는 경우, 부족한 수만큼 중복 카드를 추가
	if (SelectedCards.Num() > 0)
	{
		return AddDuplicateCardsToFillAmount(SelectedCards, InAmount);
	}

	// 선택된 카드가 없는 경우, 보상 카드 반환
	return HandleNoFilteredCards(InAmount);
}

void UChanceManagerComponent::ApplyTagWeightsToFilteredCards(TArray<FCard>& FilteredCards, const TMap<FGameplayTag, float>& TagWeights)
{
	// 가중치가 적용된 태그 목록을 가져옴
	TArray<FGameplayTag> TagWeightKeys;
	TagWeights.GetKeys(TagWeightKeys);

	// 가중치 태그로 필터링할 태그 컨테이너 생성
	FGameplayTagContainer TagWeightKeyContainer = FGameplayTagContainer::CreateFromArray(TagWeightKeys);

	// 가중치가 있는 태그를 포함하는 카드들로 필터링
	FilteredCards = GetFilteredCardsWithAnyTag(TagWeightKeyContainer, FilteredCards, true);

	// 필터링된 카드를 무작위로 섞음
	UFunctionLibrary_ArrayUtils::ShuffleArray(FilteredCards);
}

TArray<FCard> UChanceManagerComponent::PickCardsByTags(TArray<FCard>& FilteredCards, const TArray<FGameplayTag>& SelectedTags)
{
	TArray<FCard> PickedCards;

	// 각 선택된 태그에 대해 해당하는 카드 선택
	for (const FGameplayTag& Tag : SelectedTags)
	{
		FCard PickedCard;
		if (FindPickedTagAmongFilteredCards(Tag, FilteredCards, true, PickedCard))
		{
			PickedCards.Add(PickedCard);
		}
	}

	return PickedCards;
}

TArray<FCard> UChanceManagerComponent::AddDuplicateCardsToFillAmount(const TArray<FCard>& SelectedCards, int32 InAmount)
{
	// 중복 카드를 추가할 배열을 초기화
	TArray<FCard> CardsWithDuplicates = SelectedCards;

	// 카드 수량이 필요한 수량보다 적을 경우 중복 카드를 추가
	while (CardsWithDuplicates.Num() < InAmount)
	{
		// 무작위로 선택된 카드를 중복으로 추가
		int32 RandomIndex;
		FCard RandomCard = UFunctionLibrary_ArrayUtils::GetRandomElementFromArray(SelectedCards, RandomIndex);
		CardsWithDuplicates.Add(RandomCard);
	}

	// 중복이 추가된 카드 배열을 반환
	return CardsWithDuplicates;
}

TArray<FCard> UChanceManagerComponent::PickRandomCardsWithoutWeights(TArray<FCard>& FilteredCards, int32 InAmount)
{
	// 필터링된 카드 배열을 무작위로 섞음
	UFunctionLibrary_ArrayUtils::ShuffleArray(FilteredCards);

	// 선택된 카드를 저장할 배열
	TArray<FCard> RandomCards;

	// 필터링된 카드가 없는 경우 빈 배열 반환
	if (FilteredCards.Num() == 0)
	{
		return RandomCards;
	}

	// InAmount만큼 무작위로 카드 선택 (필터링된 카드 수를 초과하지 않도록 모듈로 연산)
	for (int32 i = 0; i < InAmount; ++i)
	{
		int32 Index = i % FilteredCards.Num();
		RandomCards.Add(FilteredCards[Index]);
	}

	// 선택된 카드 배열 반환
	return RandomCards;
}

TArray<FCard> UChanceManagerComponent::HandleNoFilteredCards(int32 InAmount)
{
	// 보상 카드 배열을 저장할 변수
	TArray<FCard> FallbackCards;

	// 보상 카드가 유효한지 확인
	if (!CardRewardDataRowHandle.IsNull())
	{
		// 데이터 테이블에서 보상 카드 찾기
		if (FCard* FoundRewardCard = CardRewardDataRowHandle.DataTable->FindRow<FCard>(CardRewardDataRowHandle.RowName, TEXT("")))
		{
			// 보상 카드 정보 복사
			FCard RewardsCard = *FoundRewardCard;
			RewardsCard.DataRow = CardRewardDataRowHandle;

			// 필요한 카드 수만큼 보상 카드로 채우기
			while (FallbackCards.Num() < InAmount)
			{
				FallbackCards.Add(RewardsCard);
			}

			return FallbackCards; // 보상 카드 반환
		}

		// 보상 카드를 찾지 못한 경우 에러 메시지 출력
		COS_SCREEN(TEXT("오류: 유효한 카드 옵션이 충분하지 않으며 유효한 백업 옵션이 없습니다."));
		return FallbackCards;
	}

	// CardRewardDataRowHandle이 설정되지 않은 경우 에러 메시지 출력
	COS_SCREEN(TEXT("DataTableRowHandle_CardsRewards을 반드시 설정해주세요!"));
	return FallbackCards;
}
