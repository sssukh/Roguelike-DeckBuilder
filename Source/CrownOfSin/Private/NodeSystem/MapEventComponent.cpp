// Fill out your copyright notice in the Description page of Project Settings.


#include "CrownOfSin/Public/NodeSystem/MapEventComponent.h"

#include "CardSystem/CardPlayer.h"
#include "CardSystem/ChanceManagerComponent.h"
#include "Core/CosGameInstance.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Libraries/FunctionLibrary_ArrayUtils.h"


// Sets default values for this component's properties
UMapEventComponent::UMapEventComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMapEventComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMapEventComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMapEventComponent::RunMapEvent(FDataTableRowHandle EventData)
{
}

bool UMapEventComponent::ChooseRandomEncounter(FMapEvent MapEventData, TMap<FGameplayTag, float> WeightedTags,
                                               FDataTableRowHandle& RandomEncounter)
{
	// WeightedTags IsEmpty()이면 첫방문, 아니라면 재방문(재방문은 내부적으로만 이루어진다, 외부에서는 껍데기만 넣는다)

	
	
	// MapEventData에서 EncounterTables를 가져온다.
	// 기존엔 처음 돌입시에만 배열가져오고 셔플했지만
	// 이걸 한번 더 한다고 해서 결과에 영향을 주지는 않을 것이다.
	TArray<UDataTable*> LocalEncounterTables = MapEventData.EncounterTables;

	UFunctionLibrary_ArrayUtils::ShuffleArray(LocalEncounterTables);
	
	// 미리 캐싱하지 않고 뒤로 뺐다.
	// bool NoRequiredTags = MapEventData.RequiredTags.Num()==0;
	//
	// bool NoTagWeights = false;
	
	

	FGameplayTag LocalPickedTag;
	
	if(!MapEventData.TagWeights.IsEmpty())
	{
		// 처음 호출해서 WeightedTags가 비어있을 때만 가져오기.
		if(WeightedTags.IsEmpty())
			WeightedTags=MapEventData.TagWeights;

		LocalPickedTag = GetRandomTagFromTagWeights(WeightedTags);
	}

	// GameInstance에서 DoneStoryEncounter가져오기.
	TArray<FDataTableRowHandle> LocalDoneStoryEncounters;
	
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetOuter());

	if(GameInstance->GetClass()->ImplementsInterface(UInterface_CardGameInstance::StaticClass()))
	{
		LocalDoneStoryEncounters = IInterface_CardGameInstance::Execute_GetDoneStoryEncountersFromInstance(GameInstance);
	}

	UDataTable* LocalEncounterTable= nullptr;
	
	TArray<FName> LocalEncounterRows;
	
	for (UDataTable* EncounterTable : LocalEncounterTables)
	{
		LocalEncounterTable = EncounterTable;
		UDataTableFunctionLibrary::GetDataTableRowNames(LocalEncounterTable,LocalEncounterRows);

		// EncounterRows 섞기
		UFunctionLibrary_ArrayUtils::ShuffleArray(LocalEncounterRows);


		// 반환점 2
		bool bSuccess = GetProperTableRow(MapEventData,LocalEncounterRows,LocalEncounterTable,LocalPickedTag,RandomEncounter, LocalDoneStoryEncounters);

		if(bSuccess)
		{
			return true;
		}
	}
	
	if(WeightedTags.Num()>1)
	{
		WeightedTags.Remove(LocalPickedTag);
		
		// returnPoint1로 돌아가기
		// ChooseRandomEncounter(MapEventData,false);

		// 무작위로 뽑은 pickTag를 다 검사해보자.
		// 지금 뽑은거 제외하고 다시 뽑아서 해봐.
		ChooseRandomEncounter(MapEventData,WeightedTags,RandomEncounter);
	}
	else
	{
		// LocalRetryandIgnoreDone이 필요한가
		// 그냥 DoneStory 무시하고 한번 더 해봐 인듯.
		// 무조건 하나라도 전달하기 위함인듯 하다.
		LocalDoneStoryEncounters.Empty();

		// returnPoint2로 돌아가기
		return GetProperTableRow(MapEventData,LocalEncounterRows,LocalEncounterTable,LocalPickedTag,RandomEncounter, LocalDoneStoryEncounters);
		
	}
	return false;
}

FGameplayTagContainer UMapEventComponent::GetEncounterTags(FDataTableRowHandle EncounterTags)
{
	return FGameplayTagContainer();
}



FGameplayTag UMapEventComponent::GetRandomTagFromTagWeights(TMap<FGameplayTag, float> WeightedTags)
{
	ACardPlayer* CardPlayer = Cast<ACardPlayer>(UGameplayStatics::GetActorOfClass(nullptr, ACardPlayer::StaticClass()));

	TArray<FGameplayTag> RandomPick = CardPlayer->ChanceManagerComponent->GetRandomTagsByWeights(WeightedTags,1);

	return RandomPick[0];
}

bool UMapEventComponent::GetProperTableRow(FMapEvent MapEventData, TArray<FName> EncounterRows, UDataTable* DataTable, FGameplayTag PickedTag,
                                           FDataTableRowHandle& RandomEncounter, TArray<FDataTableRowHandle> DoneStoryEncounters)
{
	for (FName EncounterRow : EncounterRows)
	{
		FDataTableRowHandle RowHandle;
		RowHandle.DataTable = DataTable;
		RowHandle.RowName = EncounterRow;
		
		FGameplayTagContainer TagContainer = GetEncounterTags(RowHandle);

		// TagWeights가 없거나 TagContainer 내부에 PickedTag가 있는 경우 true
		// PickedTag는 TagWeights에서 변형된 WeightedTags에서 랜덤하게 뽑아낸 FGameplayTag이다.
		// TagContainer는 
		bool TagContainerHasPickedTagOrNoTagWeights = MapEventData.TagWeights.IsEmpty()||TagContainer.HasTag(PickedTag);

		bool TagContainerHasAllRequiredTagsOrNoRequiredTags = TagContainer.HasAll(MapEventData.RequiredTags);

		if(TagContainerHasPickedTagOrNoTagWeights&&TagContainerHasAllRequiredTagsOrNoRequiredTags)
		{
			FDataTableRowHandle TableRowHandleforCheck;
			TableRowHandleforCheck.DataTable = DataTable;
			TableRowHandleforCheck.RowName = EncounterRow;

			
			if(!DoneStoryEncounters.Contains(TableRowHandleforCheck))
			{
				// bool bSuccess = true, FDataTableRowHandle RandomEncounter 도 반환해준다.
				RandomEncounter = TableRowHandleforCheck;
				return true;
			}
				
		}
	}
	return  false;
}




