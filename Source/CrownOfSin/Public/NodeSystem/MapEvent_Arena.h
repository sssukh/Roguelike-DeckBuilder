// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapEventComponent.h"
#include "MapEvent_Arena.generated.h"


UCLASS(ClassGroup=(Custom), Blueprintable,meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UMapEvent_Arena : public UMapEventComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMapEvent_Arena();
	

	virtual FGameplayTagContainer GetEncounterTags(FDataTableRowHandle EncounterTags) override;

	virtual void RunMapEvent(FDataTableRowHandle EventData) override;

	void ChangeLevel(FDataTableRowHandle EventData);
};
