// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapEventComponent.h"
#include "MapEvent_Story.generated.h"


UCLASS(ClassGroup=("Cos"), Blueprintable,meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UMapEvent_Story : public UMapEventComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMapEvent_Story();

public:
	virtual FGameplayTagContainer GetEncounterTags(const FDataTableRowHandle& StoryEncounter) override;

	virtual void RunMapEvent(FDataTableRowHandle EventData) override;

	
	/*
	/* Variables
	 */

	// 초기값 DataTable : DT_StoryEnounter, RowName : Filler
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "MapEvent Story")
	FDataTableRowHandle FillerEncounter;
};
