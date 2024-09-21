#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Components/SlateWrapperTypes.h"
#include "MVVM_CardVisual.generated.h"

class ACardBase;

/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UMVVM_CardVisual : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UMVVM_CardVisual();

	void UpdateCardWidget();

	void UpdateManaCrystal();

public:
	UFUNCTION(BlueprintCallable)
	void SetManaCost(const int32& NewManaCost);

	UFUNCTION(BlueprintPure)
	int32 GetManaCost() const { return ManaCost; }

	UFUNCTION(BlueprintCallable)
	void SetManaCostColor(const FSlateColor& NewManaCostColor);

	// UFUNCTION(BlueprintPure, FieldNotify)
	// const float GetHealthPercent() const { return Health / DefaultHealth; }


	UFUNCTION(BlueprintPure, FieldNotify)
	ESlateVisibility ManaCrystalOverlayVisibility() const { return ManaCost > 0 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden; }

private:
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true", ExposeOnSpawn="true"))
	ACardBase* CardActor;

	UPROPERTY(FieldNotify, Setter, Getter)
	int32 ManaCost;

	UPROPERTY(FieldNotify, Setter)
	FSlateColor ManaCostColor;

	UPROPERTY(FieldNotify)
	FSlateColor CardFrameTint;

	UPROPERTY(FieldNotify)
	UTexture2D* FrameImage;
	
	UPROPERTY(FieldNotify)
	UTexture2D* CardImage;
	
	UPROPERTY(FieldNotify)
	FText CardText;
};
