// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetingComponent.h"
#include "TargetingComponent_DiscoverArtifact.generated.h"

/*
 *	아티팩트 보상 화면에 선택된 아티팩트가 나타납니다.
 *	확률 조정을 위한 Rarity Weights는 UsedData에서 설정 가능합니다.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UTargetingComponent_DiscoverArtifact : public UTargetingComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTargetingComponent_DiscoverArtifact();

	virtual bool FindValidTargets(TArray<AActor*>& SpecifiedTargets, const FCardEffect& CardEffect, ACardBase* Card, bool bPreview, TArray<AActor*>& ValidTargets) override;

	UFUNCTION(BlueprintCallable, Category="Targeting Discover Artifact")
	void BindToArtifactConfirm(UUW_ArtifactRewardScreen* InCardRewardScreen);

	UFUNCTION(BlueprintCallable, Category="Targeting Discover Artifact")
	void ValidateAndTransferArtifact(bool bSkipped, UStatusComponent* Artifact);

	// Member Variables
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting Discover Artifact")
	TObjectPtr<AActor> TempArtifactHolder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting Discover Artifact")
	FDataTableRowHandle ArtifactOptionData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting Discover Artifact")
	FStatusData PickedArtifact;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting Discover Artifact")
	UUW_ArtifactRewardScreen* CardRewardScreen;
};
