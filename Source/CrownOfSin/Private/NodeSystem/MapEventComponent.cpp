#include "CrownOfSin/Public/NodeSystem/MapEventComponent.h"

#include "BlueprintGameplayTagLibrary.h"
#include "CardSystem/CardPlayer.h"
#include "CardSystem/ChanceManagerComponent.h"
#include "Core/CosGameInstance.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Libraries/FunctionLibrary_ArrayUtils.h"
#include "Libraries/FunctionLibrary_Singletons.h"


// Sets default values for this component's properties
UMapEventComponent::UMapEventComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}


// Called when the game starts
void UMapEventComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UMapEventComponent::RunMapEvent(FDataTableRowHandle EventData)
{
}

FGameplayTagContainer UMapEventComponent::GetEncounterTags(const FDataTableRowHandle& EncounterTags)
{
	return FGameplayTagContainer();
}

bool UMapEventComponent::ChooseRandomEncounter(const FMapEvent& MapEventData, FDataTableRowHandle& OutRandomEncounter)
{
	// Encounter 테이블을 복사하고 섞습니다.
	TArray<UDataTable*> ShuffledEncounterTables = MapEventData.EncounterTables;
	UFunctionLibrary_ArrayUtils::ShuffleArray(ShuffledEncounterTables);

	// 필요한 태그들을 가져옵니다.
	TArray<FGameplayTag> GameplayTags;
	MapEventData.RequiredTags.GetGameplayTagArray(GameplayTags);

	// 필수 태그가 있는지 여부를 확인합니다.
	bool bNoRequiredTags = GameplayTags.IsEmpty();
	bool bNoTagWeights = false;

	// 태그 가중치는 한번만 복사하여 이후 루프에서 계속 유지되도록 설정합니다.
	TMap<FGameplayTag, float> CurrentTagWeights = MapEventData.TagWeights;
	FGameplayTag SelectedTag;

	// 현재 게임 인스턴스에서 이미 완료된 Encounter를 가져옵니다.
	UGameInstance* CardGameInstance = UFunctionLibrary_Singletons::GetCardGameInstance(this);
	TArray<FDataTableRowHandle> CompletedStoryEncounters = IInterface_CardGameInstance::Execute_GetDoneStoryEncountersFromInstance(CardGameInstance);

	// Encounter를 찾을 때까지 무한 루프를 돌립니다.
	while (true)
	{
		// 태그 가중치가 존재하는 경우
		if (CurrentTagWeights.Num() > 0)
		{
			// ACardPlayer를 통해 가중치 기반 랜덤 태그 선택
			ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
			TArray<FGameplayTag> PickedTags = CardPlayer->ChanceManagerComponent->GetRandomTagsByWeights(CurrentTagWeights, 1);
			SelectedTag = PickedTags[0];
		}
		else
		{
			// 태그 가중치가 없는 경우, 태그가 없어도 유효한 Encounter를 찾을 수 있도록 플래그 설정
			bNoTagWeights = true;
		}

		UDataTable* LocalEncounterTable = nullptr;
		TArray<FName> LocalEncounterRows;

		// Encounter 테이블에서 Encounter를 하나씩 검사합니다.
		for (UDataTable* EncounterTable : ShuffledEncounterTables)
		{
			LocalEncounterTable = EncounterTable;

			// Encounter 테이블의 모든 행을 섞습니다.
			LocalEncounterRows = EncounterTable->GetRowNames();
			UFunctionLibrary_ArrayUtils::ShuffleArray(LocalEncounterRows);

			// Encounter를 찾는 시도
			if (TryFindValidEncounter(LocalEncounterTable, LocalEncounterRows,
			                          SelectedTag, bNoTagWeights, bNoRequiredTags,
			                          CompletedStoryEncounters, MapEventData.RequiredTags,
			                          OutRandomEncounter))
			{
				// 유효한 Encounter를 찾았을 때 true 반환
				return true;
			}
		}

		// 남아있는 태그가 1개 이상인 경우 태그 가중치에서 선택된 태그를 제거한 후 다시 시도
		if (CurrentTagWeights.Num() > 1)
		{
			CurrentTagWeights.Remove(SelectedTag); // 선택된 태그 제거
		}
		else
		{
			// Encounter 완료 리스트를 무시하고 한 번 더 시도합니다.
			CompletedStoryEncounters.Empty();

			// Encounter 재시도
			if (TryFindValidEncounter(LocalEncounterTable, LocalEncounterRows,
			                          SelectedTag, bNoTagWeights, bNoRequiredTags,
			                          CompletedStoryEncounters, MapEventData.RequiredTags,
			                          OutRandomEncounter))
			{
				// Encounter를 성공적으로 찾았을 때 true 반환
				return true;
			}

			// Encounter를 찾지 못했을 경우 false 반환
			return false;
		}
	}
}

bool UMapEventComponent::TryFindValidEncounter(UDataTable* EncounterTable, const TArray<FName>& EncounterRows, const FGameplayTag& PickedTag, bool bNoTagWeights, bool bNoRequiredTags,
                                               const TArray<FDataTableRowHandle>& DoneStoryEncounters, const FGameplayTagContainer& RequiredTags, FDataTableRowHandle& OutRandomEncounter)
{
	// Encounter 테이블의 각 행에 대해 유효성을 검사합니다.
	for (FName EncounterRow : EncounterRows)
	{
		FDataTableRowHandle EncounterRowHandle;
		EncounterRowHandle.DataTable = EncounterTable;
		EncounterRowHandle.RowName = EncounterRow;

		// 해당 Encounter 행에 포함된 태그들을 가져옵니다.
		FGameplayTagContainer EncounterTagContainer = GetEncounterTags(EncounterRowHandle);

		// 선택된 태그가 포함되어 있거나, 태그 가중치가 없는 경우 true로 설정
		bool bHasPickedTag = UBlueprintGameplayTagLibrary::HasTag(EncounterTagContainer, PickedTag, true) || bNoTagWeights;

		// 필수 태그들이 모두 포함되어 있거나, 필수 태그가 없는 경우 true로 설정
		bool bHasRequiredTags = UBlueprintGameplayTagLibrary::HasAllTags(EncounterTagContainer, RequiredTags, true) || bNoRequiredTags;

		// Encounter가 선택된 태그와 필수 태그를 모두 만족하는 경우
		if (bHasPickedTag && bHasRequiredTags)
		{
			// 해당 Encounter가 완료되지 않은 경우에만 유효
			if (!DoneStoryEncounters.Contains(EncounterRowHandle))
			{
				OutRandomEncounter = EncounterRowHandle;
				return true; // 유효한 Encounter를 찾았을 때 true 반환
			}
		}
	}

	// 유효한 Encounter를 찾지 못했을 경우 false 반환
	return false;
}

// bool UMapEventComponent::ChooseRandomEncounter(FMapEvent MapEventData, TMap<FGameplayTag, float> WeightedTags, FDataTableRowHandle& RandomEncounter)
// {
// 	// WeightedTags IsEmpty()이면 첫방문, 아니라면 재방문(재방문은 내부적으로만 이루어진다, 외부에서는 껍데기만 넣는다)
//
// 	// MapEventData에서 EncounterTables를 가져온다.
// 	// 기존엔 처음 돌입시에만 배열가져오고 셔플했지만
// 	// 이걸 한번 더 한다고 해서 결과에 영향을 주지는 않을 것이다.
// 	TArray<UDataTable*> LocalEncounterTables = MapEventData.EncounterTables;
//
// 	UFunctionLibrary_ArrayUtils::ShuffleArray(LocalEncounterTables);
//
// 	// 미리 캐싱하지 않고 뒤로 뺐다.
// 	// bool NoRequiredTags = MapEventData.RequiredTags.Num()==0;
// 	//
// 	// bool NoTagWeights = false;
//
//
// 	FGameplayTag LocalPickedTag;
// 	
// 	if (!MapEventData.TagWeights.IsEmpty())
// 	{
// 		// 처음 호출해서 WeightedTags가 비어있을 때만 가져오기.
// 		if (WeightedTags.IsEmpty())
// 			WeightedTags = MapEventData.TagWeights;
//
// 		LocalPickedTag = GetRandomTagFromTagWeights(WeightedTags);
// 	}
//
// 	// GameInstance에서 DoneStoryEncounter가져오기.
// 	TArray<FDataTableRowHandle> LocalDoneStoryEncounters;
//
// 	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetOuter());
//
// 	if (GameInstance->GetClass()->ImplementsInterface(UInterface_CardGameInstance::StaticClass()))
// 	{
// 		LocalDoneStoryEncounters = IInterface_CardGameInstance::Execute_GetDoneStoryEncountersFromInstance(GameInstance);
// 	}
//
// 	UDataTable* LocalEncounterTable = nullptr;
//
// 	TArray<FName> LocalEncounterRows;
//
// 	for (UDataTable* EncounterTable : LocalEncounterTables)
// 	{
// 		LocalEncounterTable = EncounterTable;
// 		UDataTableFunctionLibrary::GetDataTableRowNames(LocalEncounterTable, LocalEncounterRows);
//
// 		// EncounterRows 섞기
// 		UFunctionLibrary_ArrayUtils::ShuffleArray(LocalEncounterRows);
//
//
// 		// 반환점 2
// 		bool bSuccess = GetProperTableRow(MapEventData, LocalEncounterRows, LocalEncounterTable, LocalPickedTag, RandomEncounter, LocalDoneStoryEncounters);
//
// 		if (bSuccess)
// 		{
// 			return true;
// 		}
// 	}
//
// 	if (WeightedTags.Num() > 1)
// 	{
// 		WeightedTags.Remove(LocalPickedTag);
//
// 		// returnPoint1로 돌아가기
// 		// ChooseRandomEncounter(MapEventData,false);
//
// 		// 무작위로 뽑은 pickTag를 다 검사해보자.
// 		// 지금 뽑은거 제외하고 다시 뽑아서 해봐.
// 		ChooseRandomEncounter(MapEventData, WeightedTags, RandomEncounter);
// 	}
// 	else
// 	{
// 		// LocalRetryandIgnoreDone이 필요한가
// 		// 그냥 DoneStory 무시하고 한번 더 해봐 인듯.
// 		// 무조건 하나라도 전달하기 위함인듯 하다.
// 		LocalDoneStoryEncounters.Empty();
//
// 		// returnPoint2로 돌아가기
// 		return GetProperTableRow(MapEventData, LocalEncounterRows, LocalEncounterTable, LocalPickedTag, RandomEncounter, LocalDoneStoryEncounters);
// 	}
// 	return false;
// }
//
// FGameplayTag UMapEventComponent::GetRandomTagFromTagWeights(TMap<FGameplayTag, float> WeightedTags)
// {
// 	ACardPlayer* CardPlayer = Cast<ACardPlayer>(UGameplayStatics::GetActorOfClass(nullptr, ACardPlayer::StaticClass()));
//
// 	TArray<FGameplayTag> RandomPick = CardPlayer->ChanceManagerComponent->GetRandomTagsByWeights(WeightedTags, 1);
//
// 	return RandomPick[0];
// }
//
// bool UMapEventComponent::GetProperTableRow(FMapEvent MapEventData, TArray<FName> EncounterRows, UDataTable* DataTable, FGameplayTag PickedTag,
//                                            FDataTableRowHandle& RandomEncounter, TArray<FDataTableRowHandle> DoneStoryEncounters)
// {
// 	for (FName EncounterRow : EncounterRows)
// 	{
// 		FDataTableRowHandle RowHandle;
// 		RowHandle.DataTable = DataTable;
// 		RowHandle.RowName = EncounterRow;
//
// 		FGameplayTagContainer TagContainer = GetEncounterTags(RowHandle);
//
// 		// TagWeights가 없거나 TagContainer 내부에 PickedTag가 있는 경우 true
// 		// PickedTag는 TagWeights에서 변형된 WeightedTags에서 랜덤하게 뽑아낸 FGameplayTag이다.
// 		// TagContainer는 
// 		bool TagContainerHasPickedTagOrNoTagWeights = MapEventData.TagWeights.IsEmpty() || TagContainer.HasTag(PickedTag);
//
// 		bool TagContainerHasAllRequiredTagsOrNoRequiredTags = TagContainer.HasAll(MapEventData.RequiredTags);
//
// 		if (TagContainerHasPickedTagOrNoTagWeights && TagContainerHasAllRequiredTagsOrNoRequiredTags)
// 		{
// 			FDataTableRowHandle TableRowHandleforCheck;
// 			TableRowHandleforCheck.DataTable = DataTable;
// 			TableRowHandleforCheck.RowName = EncounterRow;
//
//
// 			if (!DoneStoryEncounters.Contains(TableRowHandleforCheck))
// 			{
// 				// bool bSuccess = true, FDataTableRowHandle RandomEncounter 도 반환해준다.
// 				RandomEncounter = TableRowHandleforCheck;
// 				return true;
// 			}
// 		}
// 	}
// 	return false;
// }
