// Fill out your copyright notice in the Description page of Project Settings.

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
UCLASS()
class CROWNOFSIN_API UUW_CardListCard : public UUserWidget, public IInterface_Utility, public IInterface_CardWidget
{
	GENERATED_BODY()
public:
	UUW_CardListCard(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;
	
	virtual void NativeConstruct() override;

	UUserWidget* SetCardVisualWidget(ACardBase* InCardActor);
	
	// Interface_Utility
public:
	virtual void Initialize_Implementation(const FGameplayTagContainer& Tags) override;

	// Interface_CardWidget
	virtual void UpdateCardWidget_Implementation(ACardBase* InCardActor) override;
	
	// Member Variables
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardListCard", meta = (BindWidget))
	TObjectPtr<USizeBox> CardVisualBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardListCard", meta = (BindWidget))
	TObjectPtr<UButton> HoverButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardListCard", meta = (BindWidget))
	TObjectPtr<UUW_ToolTipList> WBP_TooltipList;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardListCard", meta = (ExposeOnSpawn))
	TObjectPtr<ACardBase> CardActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardListCard")
	TObjectPtr<UWidget> PairedWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardListCard")
	bool bBlockHoverAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardListCard")
	TObjectPtr<UUserWidget> CardVisual;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardListCard | Config")
	TSubclassOf<UUserWidget> CardVisualClass;
	
	// Delegate
public:
	FOnCardClicked OnCardClicked;
};
