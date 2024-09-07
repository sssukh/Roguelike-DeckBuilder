#pragma once

#include "CoreMinimal.h"
#include "NodeEnumStruct.h"
#include "Components/ActorComponent.h"
#include "MapEventComponent.generated.h"


UCLASS(ClassGroup=("Cos"), meta=(BlueprintSpawnableComponent))

class CROWNOFSIN_API UMapEventComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMapEventComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	virtual void RunMapEvent(FDataTableRowHandle EventData);

	/*
	 * 인카운터의 태그를 가져오기
	 * 컴포넌트를 상속받은 곳에서 따로 정의된다.
	 */
	UFUNCTION(BlueprintCallable, Category="Map Event Component Event")
	virtual FGameplayTagContainer GetEncounterTags(const FDataTableRowHandle& EncounterTags);


	// 선택된 태그와 필수 태그를 기반으로 Encounter를 랜덤하게 선택합니다. 
	// Encounter 완료 리스트에 속하지 않은 Encounter를 찾고, 유효한 Encounter를 반환합니다.
	UFUNCTION(BlueprintCallable, Category="Map Event Component Event")
	bool ChooseRandomEncounter(const FMapEvent& MapEventData, FDataTableRowHandle& OutRandomEncounter);

protected:
	// Encounter 테이블에서 유효한 Encounter를 찾아 반환합니다. 
	// 선택된 태그와 필수 태그를 기반으로 Encounter를 필터링하고, 완료되지 않은 Encounter를 반환합니다.
	bool TryFindValidEncounter(UDataTable* EncounterTable, const TArray<FName>& EncounterRows, const FGameplayTag& PickedTag, bool bNoTagWeights, bool bNoRequiredTags,
	                           const TArray<FDataTableRowHandle>& DoneStoryEncounters, const FGameplayTagContainer& RequiredTags, FDataTableRowHandle& OutRandomEncounter);


	// 	/*
	// 	 * 임의의 인카운터 골라내기
	// 	 */
	// 	UFUNCTION(BlueprintCallable)
	// 	bool ChooseRandomEncounter(FMapEvent MapEventData, TMap<FGameplayTag, float> WeightedTags,
	// 	                           FDataTableRowHandle& RandomEncounter);
	//
	// 	
	// protected:
	// 	/*
	// 	 * WeightedTags에 들어있는 (가중치)/(총 가중치) 에 따른 확률로 Tag를 가져온다.
	// 	 */ 
	// 	FGameplayTag GetRandomTagFromTagWeights(TMap<FGameplayTag,float> WeightedTags);
	//
	// 	/*
	// 	 * 지정한 조건과 일치하는 데이터를 받아온다.
	// 	 * ChooseRandomEncounter 내부에서 호출된다.
	// 	 */
	// 	bool GetProperTableRow(FMapEvent MapEventData, TArray<FName> EncounterRows, UDataTable* DataTable, FGameplayTag PickedTag, FDataTableRowHandle& RandomEncounter, TArray
	// 	                       <FDataTableRowHandle> DoneStoryEncounters);
};
