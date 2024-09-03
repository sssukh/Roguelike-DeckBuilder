// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/CosEnumStruct.h"
#include "UObject/Interface.h"
#include "Interface_CardGameInstance.generated.h"

class ACardBase;
// This class does not need to be modified.
UINTERFACE()
class UInterface_CardGameInstance : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CROWNOFSIN_API IInterface_CardGameInstance
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Game Instance")
	TArray<int32> GetVisitedMapNodesFromInstance();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Game Instance")
	TArray<FDataTableRowHandle> GetDoneStoryEncountersFromInstance();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Game Instance")
	bool AttemptSaveGame(const FString& InLevelNameOverride, bool bIsUpdateDeck);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Game Instance")
	bool AttemptLoadGame(const FString& InSaveId, bool bOpenLevelIfValid);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Game Instance")
	void ResetGame();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Game Instance")
	bool AddPersistentHeroToInstance(FDataTableRowHandle HeroData, FDataTableRowHandle HeroDeck, FString& OutUniqueID);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Game Instance")
	bool AddArtifactToInstance(const FStatusData& InArtifact);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Game Instance")
	bool GetHeroWithIdFromInstance(const FString& InUniqueID, FMinion& OutHero, int32& OutArrayIndex);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Game Instance")
	FString GetCurrentNodeMapFromInstance();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Game Instance")
	void SetGlobalGraphicsQuality(int32 InQuality);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Game Instance")
	int32 GetGlobalGraphicsQuality();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Game Instance")
	void UpdateDeckInInstance(const TArray<ACardBase*>& InCards);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Game Instance")
	void AddVisitedNodeToInstance(int32 InNodeID);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Game Instance")
	TArray<FMinion> GetPersistentHeroesFromInstance();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Game Instance")
	bool IsDebugMode();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Game Instance")
	TArray<FStatusData> GetArtifactsFromInstance();
};
