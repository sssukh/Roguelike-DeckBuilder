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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReturnSelectedCard, ACardBase*, Card);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class CROWNOFSIN_API UUW_CardSelectorList : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_CardSelectorList(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;
	
	virtual void NativeConstruct() override;
public:

	UFUNCTION(BlueprintCallable, Category = "CardSelector")
	void UpdateCardList(const TArray<ACardBase*>& Cards, const FText& Title);

	UFUNCTION(BlueprintCallable, Category = "CardSelector")
	void ReturnCardWhenClicked(UUW_CardListCard* CardListCard, ACardBase* CardActor);

protected:
	UFUNCTION()
	void OnClicked_VisibilityButton();


	
	 /*========================================================================================
	  *	Field Members
	  =========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card Seletor List|Designer", meta = (BindWidget))
	UUniformGridPanel* CardPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card Seletor List|Designer", meta = (BindWidget))
	UOverlay* ListOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card Seletor List|Designer", meta = (BindWidget))
	UTextBlock* PileName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card Seletor List|Designer", meta = (BindWidget))
	UButton* VisibilityButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card Seletor List|Designer", meta = (BindWidget))
	UTextBlock* VisibilityButtonText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card Seletor List|Designer", meta = (BindWidgetAnim),Transient)
	UWidgetAnimation* Attention;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card Seletor List|Config|Setup")
	TSubclassOf<UUW_CardListCard> CardListClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card Seletor List")
	int32 RowLength = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card Seletor List")
	bool bDebugMode;

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "UW Card Seletor List|Delegate")
	FOnReturnSelectedCard OnReturnSelectedCard;
};