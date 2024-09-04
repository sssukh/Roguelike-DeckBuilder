#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Core/CosEnumStruct.h"
#include "NodeEnumStruct.generated.h"


// UENUM(BlueprintType)
// enum class EHeight : uint8
// {
// 	Default,
// 	OneLevel,
// 	Multilevel
// };

USTRUCT(BlueprintType)
struct FMapEvent : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map Event")
	TSubclassOf<UActorComponent> MapEventClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map Event")
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map Event")
	FDataTableRowHandle SpecificEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map Event")
	TArray<UDataTable*> EncounterTables;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map Event")
	TMap<FGameplayTag, float> TagWeights;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map Event")
	FGameplayTagContainer RequiredTags;
};

USTRUCT(BlueprintType)
struct FEncounter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Encounter")
	FGameplayTagContainer GameplayTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Encounter", meta = (RowType="/Script/CrownOfSin.Minion"))
	TArray<FDataTableRowHandle> Minions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Encounter", meta = (RowType="/Script/CrownOfSin.Card"))
	FDataTableRowHandle Reward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Encounter")
	FString Level = FString(TEXT("Arena"));
};


USTRUCT(BlueprintType)
struct FStoryEncounter : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Story Encounter")
	FGameplayTagContainer GameplayTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Story Encounter")
	FText Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Story Encounter")
	TObjectPtr<UTexture2D> Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Story Encounter")
	TArray<FCard> EncounterOptions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Story Encounter")
	TSubclassOf<UUserWidget> EncounterWidget;
};