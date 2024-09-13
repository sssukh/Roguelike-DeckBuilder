// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Widget/Interface_CardWidget.h"
#include "UW_CardVisual.generated.h"

/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_CardVisual : public UUserWidget, public IInterface_CardWidget
{
	GENERATED_BODY()
public:
	UUW_CardVisual(const FObjectInitializer& ObjectInitializer);

	
	
	// IInterface_CardWidget
	virtual void AnimateCardAppear_Implementation(EUMGSequencePlayMode::Type PlayMode) override;

	virtual void UpdateCardWidget_Implementation(ACardBase* CardActor) override;

	// Member Variables
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardVisual", Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Appear;
};
