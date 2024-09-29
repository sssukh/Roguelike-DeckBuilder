// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Status_Artifact.h"
#include "Status_Artifact_CardReward.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Artifact_CardReward : public UStatus_Artifact
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatus_Artifact_CardReward();

	virtual bool Interact_Implementation(const FGameplayTagContainer& Tags) override;

	virtual int32 AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, bool bRefreshAppearance, UObject* InPayload) override;

	// Member Variables
public:
	UPROPERTY(EditAnywhere, BlueprintreadWrite)
	FDataTableRowHandle CardRewardData;

	UPROPERTY(EditAnywhere, BlueprintreadWrite)
	FDataTableRowHandle DefaultRewardData;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDataTable> CardsRewardTable;
};
