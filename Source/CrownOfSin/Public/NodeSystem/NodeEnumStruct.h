#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
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
	TObjectPtr<UActorComponent> MapEventClass;

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
