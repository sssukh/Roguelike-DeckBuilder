#include "Core/CosSaveGame.h"

UCosSaveGame::UCosSaveGame()
{
}

void UCosSaveGame::SavePersistentData(const TArray<FMinion>& InPersistentHeroes, const TArray<FCard>& InDeck, const TArray<int32>& InVisitedNodes, const TArray<FStatusData>& InArtifacts,
                                      const FString& InLevelName, const FEncounterData& InCurrentEncounter, const FString& InCurrentNodeMap, const TArray<UDataTable*>& InRewardTables,
                                      const FGameplayTagContainer& InAllowedCardRewardTags,
                                      const TArray<FDataTableRowHandle>& InDoneStoryEncounters)
{
	PersistentHeroes = InPersistentHeroes;
	Deck = InDeck;
	VisitedNodes = InVisitedNodes;
	Artifacts = InArtifacts;
	LevelName = InLevelName;
	CurrentEncounter = InCurrentEncounter;
	CurrentNodeMap = InCurrentNodeMap;
	RewardTables = InRewardTables;
	AllowedCardRewardTags = InAllowedCardRewardTags;
	DoneStoryEncounters = InDoneStoryEncounters;
}
