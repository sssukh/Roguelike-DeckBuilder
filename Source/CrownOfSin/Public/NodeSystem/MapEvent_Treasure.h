// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapEventComponent.h"
#include "MapEvent_Treasure.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UMapEvent_Treasure : public UMapEventComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMapEvent_Treasure();

	// UFUNCTION(BlueprintCallable, Category = "MapEvent Treasure")
	virtual FGameplayTagContainer GetEncounterTags(UDataTable* DataTable, FName RowName) override;

	// UFUNCTION(BlueprintCallable, Category = "MapEvent Treasure")
	virtual void RunMapEvent(FDataTableRowHandle EventData) override;

	void RunEvent(FGameplayTag EventTag, UObject* CallingObject, bool bGlobal, UObject* Payload, FGameplayTagContainer CallTags);
	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MapEvent Treasure")
	FDataTableRowHandle FillerEncounter;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MapEvent Treasure")
	FDataTableRowHandle RewardCardData;
};
