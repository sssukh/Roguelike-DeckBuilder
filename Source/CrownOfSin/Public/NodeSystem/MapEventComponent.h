
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
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	virtual void RunMapEvent(FDataTableRowHandle EventData);

	/*
	 * 임의의 인카운터 골라내기
	 */
	UFUNCTION(BlueprintCallable)
	bool ChooseRandomEncounter(FMapEvent MapEventData, TMap<FGameplayTag, float> WeightedTags,
	                           FDataTableRowHandle& RandomEncounter);

	/*
	 * 인카운터의 태그를 가져오기
	 * 컴포넌트를 상속받은 곳에서 따로 정의된다.
	 */
	UFUNCTION(BlueprintCallable)
	virtual FGameplayTagContainer GetEncounterTags(UDataTable* DataTable, FName RowName);

	/*
	 * WeightedTags에 들어있는 (가중치)/(총 가중치) 에 따른 확률로 Tag를 가져온다.
	 */ 
	FGameplayTag GetRandomTagFromTagWeights(TMap<FGameplayTag,float> WeightedTags);

	/*
	 * 지정한 조건과 일치하는 데이터를 받아온다.
	 * ChooseRandomEncounter 내부에서 호출된다.
	 */
	bool GetProperTableRow(FMapEvent MapEventData, TArray<FName> EncounterRows, UDataTable* DataTable, FGameplayTag PickedTag, FDataTableRowHandle& RandomEncounter, TArray
	                       <FDataTableRowHandle> DoneStoryEncounters);
	
};
