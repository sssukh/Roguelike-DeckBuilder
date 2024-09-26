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
 * UUW_CardVisual 클래스는 카드의 시각적 정보를 UI에서 표시하는 위젯입니다.
 * 이 위젯은 카드의 이미지, 마나 코스트, 카드 이름, 설명 등을 화면에 나타내며, 카드의 상태에 따라 다양한 애니메이션을 재생할 수 있는 기능을 제공합니다.
 * 또한, 카드의 상태(예: Glow)와 같은 시각적 효과를 활성화하거나 비활성화할 수 있습니다.
 * 
 * 주요 기능:
 * - 카드의 이미지, 텍스트, 마나 코스트 등의 정보를 표시
 * - 카드가 특정 상황에서 강조(Glow)될 수 있는 기능 제공
 * - 카드의 등장, 알림, 소모 등의 애니메이션을 지원
 * - 카드의 설명을 실시간으로 갱신하며, 카드의 효과 값도 표시 가능
 */
UCLASS()
class CROWNOFSIN_API UUW_CardVisual : public UUserWidget, public IInterface_CardWidget
{
	GENERATED_BODY()

public:
	UUW_CardVisual(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

public:
	// 카드의 효과 값을 대체하여 설명 텍스트를 업데이트하는 함수
	UFUNCTION(BlueprintCallable, Category="UW Card Visual|Design")
	void ReplacePowerValues();

	// 마나 크리스탈 정보를 업데이트하는 함수
	UFUNCTION(BlueprintCallable, Category="UW Card Visual|Design")
	void UpdateManaCrystal();

protected:
	// 카드 프레임에 동일한 틴트를 적용하는 함수
	UFUNCTION(BlueprintCallable, Category="UW Card Visual|Design")
	void SetCardFrameTint(const FSlateColor& TintColor);

	// 희귀성 보석을 업데이트하는 함수
	UFUNCTION(BlueprintCallable, Category="UW Card Visual|Design")
	void UpdateRarityGem();

	//마나 비용의 변화 방향에 따라 마나 비용 텍스트의 색상을 설정합니다.
	void SetManaCostColor(int32 CostChangeDirection);


	/*========================================================================================
	 *	IInterface_CardWidget
	 =========================================================================================*/
public:
	// 카드의 Glow 효과를 활성화하거나 비활성화하는 함수
	virtual void ToggleCardGlow_Implementation(bool bEnable, FLinearColor Color) override;

	// 카드 위젯을 카드 데이터를 기반으로 업데이트하는 함수
	virtual void UpdateCardWidget_Implementation(ACardBase* InCardActor) override;

	// 카드 등장 애니메이션을 재생하는 함수
	virtual void AnimateCardAppear_Implementation(EUMGSequencePlayMode::Type PlayMode) override;

	// 카드 알림 애니메이션을 재생하는 함수
	virtual void AnimateCardNotify_Implementation(EUMGSequencePlayMode::Type PlayMode) override;

	// 카드 소모(Exhaust) 애니메이션을 재생하는 함수
	virtual void AnimateCardExhaust_Implementation(EUMGSequencePlayMode::Type PlayMode) override;


	/*========================================================================================
	 *	Field Members
	 =========================================================================================*/
public:
	UPROPERTY(BlueprintReadWrite, Category = "UW Card Visual|Designer", meta = (BindWidget))
	TObjectPtr<UOverlay> ManaCrystalOverlay;

	UPROPERTY(BlueprintReadWrite, Category = "UW Card Visual|Designer", meta = (BindWidget))
	TObjectPtr<UBorder> GlowBorder;

	UPROPERTY(BlueprintReadWrite, Category = "UW Card Visual|Designer", meta = (BindWidget))
	TObjectPtr<UImage> TypeBanner;

	UPROPERTY(BlueprintReadWrite, Category = "UW Card Visual|Designer", meta = (BindWidget))
	TObjectPtr<UImage> RarityGem;

	UPROPERTY(BlueprintReadWrite, Category = "UW Card Visual|Designer", meta = (BindWidget))
	TObjectPtr<UImage> ManaCrystalFrame;

	UPROPERTY(BlueprintReadWrite, Category = "UW Card Visual|Designer", meta = (BindWidget))
	TObjectPtr<UImage> GemShadow;

	UPROPERTY(BlueprintReadWrite, Category = "UW Card Visual|Designer", meta = (BindWidget))
	TObjectPtr<UImage> FrameImage;

	UPROPERTY(BlueprintReadWrite, Category = "UW Card Visual|Designer", meta = (BindWidget))
	TObjectPtr<UImage> CardImage;

	UPROPERTY(BlueprintReadWrite, Category = "UW Card Visual|Designer", meta = (BindWidget))
	TObjectPtr<UTextBlock> CardName;

	UPROPERTY(BlueprintReadWrite, Category = "UW Card Visual|Designer", meta = (BindWidget))
	TObjectPtr<UTextBlock> ManaCost;

	UPROPERTY(BlueprintReadWrite, Category = "UW Card Visual|Designer", meta = (BindWidget))
	TObjectPtr<UTextBlock> TypeText;

	UPROPERTY(BlueprintReadWrite, Category = "UW Card Visual|Designer", meta = (BindWidget))
	TObjectPtr<URichTextBlock> RichDescription;

	UPROPERTY(BlueprintReadWrite, Category = "UW Card Visual|Designer", Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> GlowPulse;

	UPROPERTY(BlueprintReadWrite, Category = "UW Card Visual|Designer", Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> HideGlow;

	UPROPERTY(BlueprintReadWrite, Category = "UW Card Visual|Designer", Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Notify;

	UPROPERTY(BlueprintReadWrite, Category = "UW Card Visual|Designer", Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Appear;

	UPROPERTY(BlueprintReadWrite, Category = "UW Card Visual|Designer", Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ExhaustUp;

public:
	// 카드의 타입 이름과 매칭되는 텍스트 맵
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card Visual|Config|Setup")
	TMap<FGameplayTag, FText> TypeNames;

	// 희귀성 보석과 매칭되는 텍스처 맵
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card Visual|Config|Setup")
	TMap<FGameplayTag, TObjectPtr<UTexture2D>> RarityGems;

	// 기본 희귀성 보석 텍스처
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card Visual|Config|Setup")
	TObjectPtr<UTexture2D> DefaultRarityGemTexture;

	// Glow 효과의 속도
	UPROPERTY(BlueprintReadWrite, Category="UW Card Visual")
	float GlowPulseSpeed = 0.8f;

	// 카드와 연관된 카드 액터 참조
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card Visual", meta=(ExposeOnSpawn="true"))
	TObjectPtr<ACardBase> CardActor;
};
