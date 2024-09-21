#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Widget/Interface_CardWidget.h"
#include "UW_CardVisual.generated.h"

class UOverlay;
class URichTextBlock;
class UTextBlock;
class UImage;
class UBorder;
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_CardVisual : public UUserWidget, public IInterface_CardWidget
{
	GENERATED_BODY()

public:
	UUW_CardVisual(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

public:
	void ReplacePowerValues();
	
	void UpdateManaCrystal();
	
	/*========================================================================================
	 *	IInterface_CardWidget
	 =========================================================================================*/
public:
	virtual void ToggleCardGlow_Implementation(bool bEnable, FColor Color) override;
	
	virtual void UpdateCardWidget_Implementation(ACardBase* InCardActor) override;

	virtual void AnimateCardAppear_Implementation(EUMGSequencePlayMode::Type PlayMode) override;

	virtual void AnimateCardNotify_Implementation() override;

	virtual void AnimateCardExhaust_Implementation(EUMGSequencePlayMode::Type PlayMode) override;
	


	// Member Variables
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Visual|Designer", meta = (BindWidget))
	UOverlay* ManaCrystalOverlay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Visual|Designer", meta = (BindWidget))
	UBorder* GlowBorder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Visual|Designer", meta = (BindWidget))
	UImage* TypeBanner;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Visual|Designer", meta = (BindWidget))
	UImage* RarityGem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Visual|Designer", meta = (BindWidget))
	UImage* ManaCrystalFrame;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Visual|Designer", meta = (BindWidget))
	UImage* GemShadow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Visual|Designer", meta = (BindWidget))
	UImage* FrameImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Visual|Designer", meta = (BindWidget))
	UImage* CardImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Visual|Designer", meta = (BindWidget))
	UTextBlock* CardName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Visual|Designer", meta = (BindWidget))
	UTextBlock* ManaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Visual|Designer", meta = (BindWidget))
	UTextBlock* TypeText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Visual|Designer", meta = (BindWidget))
	URichTextBlock* RichDescription;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Visual|Designer", Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> GlowPulse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Visual|Designer", Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> HideGlow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Visual|Designer", Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Notify;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Visual|Designer", Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Appear;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Visual|Designer", Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ExhaustUp;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Card Visual|Config|Setup")
	TMap<FGameplayTag, FText> TypeNames;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Card Visual|Config|Setup")
	TMap<FGameplayTag, UTexture2D*> RarityGems;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Card Visual|Config|Setup")
	 UTexture2D* DefaultRarityGemTexture;

	UPROPERTY(BlueprintReadWrite, Category="Card Visual")
	float GlowPulseSpeed = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Card Visual", meta=(ExposeOnSpawn="true"))
	ACardBase* CardActor;
};
