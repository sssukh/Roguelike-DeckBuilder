﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Interface_Utility.h"
#include "Interfaces/Widget/Interface_CardWidget.h"
#include "UW_CardListCard.generated.h"

class UUW_ToolTipList;
class UButton;
class USizeBox;
class ACardBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCardClicked, UUW_CardListCard*, CardListCard, ACardBase*, CardActor);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class CROWNOFSIN_API UUW_CardListCard : public UUserWidget, public IInterface_Utility, public IInterface_CardWidget
{
	GENERATED_BODY()

public:
	UUW_CardListCard(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable,Category="UW Card List Card Event")
	UUserWidget* SetCardVisualWidget(ACardBase* InCardActor);


	
	// Interface_Utility
public:
	virtual void Initialize_Implementation(const FGameplayTagContainer& Tags) override;

	// Interface_CardWidget
public:
	virtual void UpdateCardWidget_Implementation(ACardBase* InCardActor) override;


	

	// Member Variables
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardListCard", meta = (BindWidget))
	TObjectPtr<USizeBox> CardVisualBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardListCard", meta = (BindWidget))
	TObjectPtr<UButton> HoverButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardListCard", meta = (BindWidget))
	TObjectPtr<UUW_ToolTipList> WBP_TooltipList;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardListCard", meta = (ExposeOnSpawn))
	TObjectPtr<ACardBase> CardActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardListCard")
	TObjectPtr<UWidget> PairedWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardListCard")
	bool bBlockHoverAnim;

	/*WBP_CardVisual 클래스입니다.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardListCard")
	TObjectPtr<UUserWidget> CardVisual;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardListCard | Config")
	TSubclassOf<UUserWidget> CardVisualClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardListCard", Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Select;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardListCard", Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOut;
	// Delegate
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "CardListCard")
	FOnCardClicked OnCardClicked;
};
