#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NodeEnumStruct.h"
#include "Components/ActorComponent.h"
#include "MapEventComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=("CrownOfSin"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UMapEventComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMapEventComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Map Event")
	void RunMapEvent(const FDataTableRowHandle& InEventData);

	UFUNCTION(BlueprintCallable, Category="Map Event")
	FGameplayTagContainer GetEncounterTags(UDataTable* InDataTable, FName InRowName);

	UFUNCTION(BlueprintCallable, Category="Map Event")
	bool ChooseRandomEncounter(FMapEvent& InMapEventData, FDataTableRowHandle& OutRandomEncounter);

protected:
	bool SelectTagAndStartEncounterSearch(FMapEvent& MapEventData, FDataTableRowHandle& OutRandomEncounter, TArray<UDataTable*>& EncounterTables,
	                                      bool bNoRequiredTags, bool& bRetryAndIgnoreDoneEncounters, TMap<FGameplayTag, float>& TagWeights);

	bool SearchEncountersWithSelectedTag(FMapEvent& MapEventData, FDataTableRowHandle& OutRandomEncounter, TArray<UDataTable*>& EncounterTables, FGameplayTag& SelectedTag,
	                                     bool bNoRequiredTags, bool& bRetryAndIgnoreDoneEncounters, bool bNoTagWeights, TMap<FGameplayTag, float>& TagWeights);
};
