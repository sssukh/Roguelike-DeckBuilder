// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_ScreenFade.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_ScreenFade : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_ScreenFade(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Occlude;
};
