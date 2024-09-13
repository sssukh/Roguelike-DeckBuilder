// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_CardSelectorList.generated.h"


class UButton;
class ACardBase;
class UUW_CardListCard;
class UOverlay;
class UUniformGridPanel;
class UTextBlock;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReturnSelectedCardInSelectorList, ACardBase*, Card);
/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class CROWNOFSIN_API UUW_CardSelectorList : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UUW_CardSelectorList(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;
	
	UFUNCTION(BlueprintCallable, Category = "CardSelector")
	void UpdateCardList(TArray<ACardBase*>& Cards, const FText& Title);

	UFUNCTION(BlueprintCallable, Category = "CardSelector")
	void ReturnCardWhenClicked(UUW_CardListCard* CardListCard, ACardBase* CardActor);

	
	
	// Member Variables
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "CardSelector", meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> CardPanel;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "CardSelector", meta = (BindWidget))
	TObjectPtr<UOverlay> ListOverlay;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "CardSelector", meta = (BindWidget))
	TObjectPtr<UTextBlock> PileName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "CardSelector", meta = (BindWidget))
	TObjectPtr<UButton> VisibilityButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "CardSelector", meta = (BindWidget))
	TObjectPtr<UTextBlock> VisibilityButtonText;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "CardSelector")
	int32 RowLength = 4;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "CardSelector")
	bool bDebugMode;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "CardSelector | Config")
	TSubclassOf<UUW_CardListCard> CardListClass;
	// Delegate
public:
	UPROPERTY(BlueprintAssignable,BlueprintCallable, Category = "CardSelector")
	FOnReturnSelectedCardInSelectorList OnReturnSelectedCardInSelectorList;
};
