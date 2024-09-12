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
UCLASS()
class CROWNOFSIN_API UUW_CardSelectorList : public UUserWidget
{
	GENERATED_BODY()
public:
	// OnClicked(VisibilityButton) 노드는 남겨둘 것.
	
	UUW_CardSelectorList(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;
	
	void UpdateCardList(TArray<ACardBase*>& Cards, const FText& Title);

	UFUNCTION(BlueprintCallable, Category = "CardSelector")
	void ReturnCardWhenClicked(UUW_CardListCard* CardListCard, ACardBase* CardActor);

	// 애니메이션이랑 visibility 건드리는 부분밖에 없다.
	// UFUNCTION(BlueprintImplementableEvent, Category = "CardSelector")
	// void VisibilityButtonClicked();
	
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

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "CardSelector", meta = (BindWidget))
	int32 RowLength = 4;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "CardSelector", meta = (BindWidget))
	bool bDebugMode;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "CardSelector")
	TSubclassOf<UUW_CardListCard> CardListClass;
	// Delegate
public:
	FOnReturnSelectedCardInSelectorList OnReturnSelectedCardInSelectorList;
};
