#pragma once

#include "CoreMinimal.h"
#include "CosEnumStruct.h"
#include "GameFramework/SaveGame.h"
#include "NodeSystem/NodeEnumStruct.h"
#include "CosSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UCosSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UCosSaveGame();

public:
	UFUNCTION(BlueprintCallable, Category="Cos Save Game Event")
	void SavePersistentData(const TArray<FMinion>& InPersistentHeroes, const TArray<FCard>& InDeck, const TArray<int32>& InVisitedNodes,
	                        const TArray<FStatusData>& InArtifacts, const FString& InLevelName, const FEncounter& InCurrentEncounter, const FString& InCurrentNodeMap,
	                        const TArray<UDataTable*>& InRewardTables, const FGameplayTagContainer& InAllowedCardRewardTags, const TArray<FDataTableRowHandle>& InDoneStoryEncounters);

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cos Save Game")
	TArray<FMinion> PersistentHeroes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cos Save Game")
	TArray<FCard> Deck;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cos Save Game")
	TArray<int32> VisitedNodes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cos Save Game")
	TArray<FStatusData> Artifacts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cos Save Game")
	FString LevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cos Save Game")
	FEncounter CurrentEncounter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cos Save Game")
	FString CurrentNodeMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cos Save Game")
	TArray<UDataTable*> RewardTables;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cos Save Game")
	FGameplayTagContainer AllowedCardRewardTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cos Save Game")
	TArray<FDataTableRowHandle> DoneStoryEncounters;
};
