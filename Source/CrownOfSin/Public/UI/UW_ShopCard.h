// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardSystem/CardBase.h"
#include "Interfaces/Widget/Interface_CardWidget.h"
#include "UW_ShopCard.generated.h"

class UImage;
class UUW_CardListCard;
class UTextBlock;
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_ShopCard : public UUserWidget, public IInterface_CardWidget
{
	GENERATED_BODY()

public:
	UUW_ShopCard(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Shop Card")
	void AttemptPurchase(UUW_CardListCard* CardListCard, ACardBase* InCardActor);

	UFUNCTION(BlueprintCallable, Category = "Shop Card")
	void UpdateFromCoinAmount(int32 CoinCount);

	virtual void UpdateCardWidget_Implementation(ACardBase* InCardActor) override;

	//Member Variables
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Card|Designer", meta = (BindWidget))
	UImage* CoinImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Card|Designer", meta = (BindWidget))
	UTextBlock* CostText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Card|Designer", meta = (BindWidget))
	UUW_CardListCard* WBP_CardListCard;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Card", meta = (ExposeOnSpawn))
	TObjectPtr<ACardBase> CardActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Card")
	int32 Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Card")
	FSlateColor CanAffordColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Card")
	FSlateColor CannotAffordColor = FSlateColor(FLinearColor(1.0f, 0.130208f, 0.130208f, 0.672f));
};
