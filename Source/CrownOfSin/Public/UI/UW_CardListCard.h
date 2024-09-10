// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_CardListCard.generated.h"

class ACardBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCardClicked, UUW_CardListCard*, CardListCard, ACardBase*, CardActor);

/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_CardListCard : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_CardListCard(const FObjectInitializer& ObjectInitializer);

	// Member Variables
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<ACardBase> CardActor;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidget> PairedWidget;
	// Delegate
public:
	FOnCardClicked OnCardClicked;
};
