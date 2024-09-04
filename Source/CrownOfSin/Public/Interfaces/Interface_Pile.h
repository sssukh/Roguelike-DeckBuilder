// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "Interface_Pile.generated.h"

class UPileComponent;
class ACardBase;
// This class does not need to be modified.
UINTERFACE()
class UInterface_Pile : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CROWNOFSIN_API IInterface_Pile
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Pile")
	bool SpawnAndAddNewCard(const FDataTableRowHandle& DataRowHandle, ACardBase*& OutCard);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Pile")
	bool AttemptAddExistingCard(ACardBase* InCard);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Pile")
	TArray<ACardBase*> GetCards();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Pile")
	void AssignExistingCards(const TArray<ACardBase*>& InDeck);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Pile")
	void TransferCardToPile(ACardBase* InCard, UObject* InTargetPile, FGameplayTag TransferTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Pile")
	bool RequestRemoveCardFromPile(ACardBase* InCard);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Pile")
	bool RequestCardFromPile(TSubclassOf<UPileComponent> RequestingPile, ACardBase* SpecificCard, bool bIsRandomCard, const FGameplayTagContainer& RequiredTags, ACardBase*& OutCard);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Pile")
	bool ClearPile();
};
