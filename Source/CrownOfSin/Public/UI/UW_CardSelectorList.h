// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_CardSelectorList.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReturnSelectedCardInSelectorList, ACardBase*, Card);
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_CardSelectorList : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_CardSelectorList(const FObjectInitializer& ObjectInitializer);

	// TODO: 구현 필요
	void UpdateCardList(TArray<ACardBase*>& Cards,const FText& Title)
	{}

	// Delegate

	FOnReturnSelectedCardInSelectorList OnReturnSelectedCardInSelectorList;
};
