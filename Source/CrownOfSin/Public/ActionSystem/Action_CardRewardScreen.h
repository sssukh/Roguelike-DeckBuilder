// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionBase.h"
#include "Action_CardRewardScreen.generated.h"

class ACardBase;

UCLASS()
class CROWNOFSIN_API AAction_CardRewardScreen : public AActionBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAction_CardRewardScreen();

	virtual void PlayAction_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Action Card Reward Screen")
	void ContinueActionOnCardSelect(bool bSkipped,ACardBase* InCard);

	// Member Variables
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Card Reward Screen")
	TArray<ACardBase*> CardOptions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Card Reward Screen")
	FText ScreenText = FText::FromString(TEXT("Choose a Card"));
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Card Reward Screen")
	bool bAllowSkip;
};
