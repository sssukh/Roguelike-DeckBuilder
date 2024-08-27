#include "NodeSystem/MapEventComponent.h"

#include "BlueprintGameplayTagLibrary.h"
#include "CardSystem/CardPlayer.h"
#include "CardSystem/ChanceManagerComponent.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/CosLog.h"
#include "Utilities/FunctionLibrary_ArrayUtils.h"


UMapEventComponent::UMapEventComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	if (UBlueprint* Blueprint = Cast<UBlueprint>(GetClass()->ClassGeneratedBy))
		Blueprint->bRunConstructionScriptOnDrag = false;
}

void UMapEventComponent::BeginPlay()
{
	Super::BeginPlay();


	// ...
}

void UMapEventComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMapEventComponent::RunMapEvent_Implementation(const FDataTableRowHandle& InEventData)
{
}

FGameplayTagContainer UMapEventComponent::GetEncounterTags(UDataTable* InDataTable, FName InRowName)
{
	return {};
}

bool UMapEventComponent::ChooseRandomEncounter(FMapEvent& MapEventData, FDataTableRowHandle& OutRandomEncounter)
{
	// 초기 설정
	TArray<UDataTable*> EncounterTables = MapEventData.EncounterTables;
	UFunctionLibrary_ArrayUtils::ShuffleArray<UDataTable*>(EncounterTables);

	TArray<FGameplayTag> RequiredTagsArray = MapEventData.RequiredTags.GetGameplayTagArray();
	bool bNoRequiredTags = RequiredTagsArray.IsEmpty(); // 필수 태그가 없는지 확인
	bool bRetryAndIgnoreDoneEncounters = false; // 완료된 엔카운터 무시하고 재시도할 플래그

	TMap<FGameplayTag, float> TagWeights;
	if (MapEventData.TagWeights.Num() > 0)
	{
		TagWeights = MapEventData.TagWeights; // 태그 가중치 초기화
	}

	// 진입점1부터 시작
	return SelectTagAndStartEncounterSearch(MapEventData, OutRandomEncounter, EncounterTables, bNoRequiredTags, bRetryAndIgnoreDoneEncounters, TagWeights);
}

// 진입점1: 태그를 선택하고 Encounter 찾기를 시작합니다.
bool UMapEventComponent::SelectTagAndStartEncounterSearch(FMapEvent& MapEventData, FDataTableRowHandle& OutRandomEncounter, TArray<UDataTable*>& EncounterTables,
                                                          bool bNoRequiredTags, bool& bRetryAndIgnoreDoneEncounters, TMap<FGameplayTag, float>& TagWeights)
{
	bool bNoTagWeights = false; // 태그 가중치가 없는지 확인
	FGameplayTag SelectedTag; // 선택된 태그

	if (TagWeights.Num() > 0)
	{
		// 진입점1: 태그 선택
		ACardPlayer* CardPlayer = Cast<ACardPlayer>(UGameplayStatics::GetActorOfClass(this, ACardPlayer::StaticClass()));
		if (!CardPlayer)
		{
			COS_LOG_SCREEN(TEXT("CardPlayer 캐스팅에 실패했습니다."));
			return false;
		}

		TArray<FGameplayTag> PickedTags = CardPlayer->ChanceManagerComponent->GetRandomTagsByWeights(TagWeights, 1);
		SelectedTag = PickedTags[0]; // 무작위로 선택된 태그
	}
	else
	{
		bNoTagWeights = true;
	}

	// Encounter 테이블을 순회하면서 진입점2를 호출
	return SearchEncountersWithSelectedTag(MapEventData, OutRandomEncounter, EncounterTables, SelectedTag, bNoRequiredTags, bRetryAndIgnoreDoneEncounters, bNoTagWeights, TagWeights);
}

bool UMapEventComponent::SearchEncountersWithSelectedTag(FMapEvent& MapEventData, FDataTableRowHandle& OutRandomEncounter, TArray<UDataTable*>& EncounterTables, FGameplayTag& SelectedTag,
                                                         bool bNoRequiredTags, bool& bRetryAndIgnoreDoneEncounters, bool bNoTagWeights, TMap<FGameplayTag, float>& TagWeights)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	TArray<FDataTableRowHandle> DoneEncounters = IInterface_CardGameInstance::Execute_GetDoneStoryEncountersFromInstance(GameInstance);

	for (UDataTable* EncounterTable : EncounterTables)
	{
		TArray<FName> EncounterRows = EncounterTable->GetRowNames();
		UFunctionLibrary_ArrayUtils::ShuffleArray<FName>(EncounterRows); // 엔카운터 행들을 무작위로 섞음

		// 진입점2: 각 Encounter 행을 순회
		for (FName EncounterRow : EncounterRows)
		{
			FGameplayTagContainer EncounterTags = GetEncounterTags(EncounterTable, EncounterRow);

			bool bTagCondition = bNoTagWeights || UBlueprintGameplayTagLibrary::HasTag(EncounterTags, SelectedTag, true);
			bool bRequiredTagsCondition = bNoRequiredTags || UBlueprintGameplayTagLibrary::HasAllTags(EncounterTags, MapEventData.RequiredTags, true);

			if (bTagCondition && bRequiredTagsCondition)
			{
				FDataTableRowHandle ValidEncounter;
				ValidEncounter.DataTable = EncounterTable;
				ValidEncounter.RowName = EncounterRow;
				if (!DoneEncounters.Contains(ValidEncounter))
				{
					OutRandomEncounter = ValidEncounter; // 유효한 Encounter 찾음
					return true;
				}
			}
		}
	}

	// 태그를 줄이고 다시 진입점1로 돌아감
	if (TagWeights.Num() > 1)
	{
		TagWeights.Remove(SelectedTag); // 선택된 태그 제거
		return SelectTagAndStartEncounterSearch(MapEventData, OutRandomEncounter, EncounterTables, bNoRequiredTags, bRetryAndIgnoreDoneEncounters, TagWeights);
	}
	else if (!bRetryAndIgnoreDoneEncounters)
	{
		// 첫 번째 재시도: 완료된 엔카운터 목록을 비우고 다시 진입점2부터 시작
		bRetryAndIgnoreDoneEncounters = true;
		DoneEncounters.Empty(); // 완료된 엔카운터 목록 비우기
		return SearchEncountersWithSelectedTag(MapEventData, OutRandomEncounter, EncounterTables, SelectedTag, bNoRequiredTags, bRetryAndIgnoreDoneEncounters, bNoTagWeights, TagWeights);
	}
	else
	{
		// 모든 시도가 실패한 경우
		OutRandomEncounter = FDataTableRowHandle();
		return false;
	}
}


// bool UMapEventComponent::ChooseRandomEncounter(FMapEvent& InMapEventData, FDataTableRowHandle& OutRandomEncounter)
// {
// 	TArray<UDataTable*> LocalEncounterTables = InMapEventData.EncounterTables;
// 	UFunctionLibrary_ArrayUtils::ShuffleArray<UDataTable*>(LocalEncounterTables);
//
// 	TArray<FGameplayTag> RequiredTagsArray = InMapEventData.RequiredTags.GetGameplayTagArray();
// 	bool bLocalNoRequiredTags = RequiredTagsArray.IsEmpty();
// 	bool bLocalNoTagWeights = false;
//
// 	bool bLocalRetryAndIgnoreDone = false;
// 	FGameplayTag LocalPickedTag;
// 	TMap<FGameplayTag, float> LocalTagWeights;
//
// 	if (InMapEventData.TagWeights.Num() > 0)
// 	{
// 		LocalTagWeights = InMapEventData.TagWeights;
//
// 		//진입점1
// 		ACardPlayer* CardPlayer = Cast<ACardPlayer>(UGameplayStatics::GetActorOfClass(this, ACardPlayer::StaticClass()));
// 		TArray<FGameplayTag> PickedTags = CardPlayer->ChanceManagerComponent->GetRandomTagsByWeights(LocalTagWeights, 1);
// 		LocalPickedTag = PickedTags[0];
// 	}
// 	else
// 	{
// 		bLocalNoTagWeights = true;
// 	}
//
// 	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
// 	TArray<FDataTableRowHandle> LocalDoneStoryEncounters = IInterface_CardGameInstance::Execute_GetDoneStoryEncountersFromInstance(GameInstance);
//
// 	for (UDataTable* LocalEncounterTable : LocalEncounterTables)
// 	{
// 		TArray<FName> LocalEncounterRows = LocalEncounterTable->GetRowNames();
// 		UFunctionLibrary_ArrayUtils::ShuffleArray<FName>(LocalEncounterRows);
//
// 		//진입점2
// 		for (FName EncounterRow : LocalEncounterRows)
// 		{
// 			FGameplayTagContainer Tags = GetEncounterTags(LocalEncounterTable, EncounterRow);
//
// 			bool bCondition1 = bLocalNoTagWeights || UBlueprintGameplayTagLibrary::HasTag(Tags, LocalPickedTag, true);
// 			bool bCondition2 = bLocalNoRequiredTags || UBlueprintGameplayTagLibrary::HasAllTags(Tags, InMapEventData.RequiredTags, true);
//
// 			if (bCondition1 && bCondition2)
// 			{
// 				FDataTableRowHandle FindEncounter;
// 				FindEncounter.DataTable = LocalEncounterTable;
// 				FindEncounter.RowName = EncounterRow;
// 				if (!LocalDoneStoryEncounters.Contains(FindEncounter))
// 				{
// 					OutRandomEncounter = FindEncounter;
// 					return true;
// 				}
// 			}
// 		}
// 	}
//
// 	if (LocalTagWeights.Num() > 1)
// 	{
// 		LocalTagWeights.Remove(LocalPickedTag);
// 		//여기서부터는 다시 진입점1부터 로직이 실행되어야 합니다.
// 	}
// 	else
// 	{
// 		if (bLocalRetryAndIgnoreDone)
// 		{
// 			OutRandomEncounter = FDataTableRowHandle();
// 			return false;
// 		}
// 		else
// 		{
// 			bLocalRetryAndIgnoreDone = true;
// 			LocalDoneStoryEncounters.Empty();
// 			//여기서부터는 다시 진입점2부터 로직이 시작되어야 합니다.
// 		}
// 	}
// }
