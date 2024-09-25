#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_ArtifactRewardScreen.generated.h"


class UTextBlock;
class UUW_StatusIcon;
class UButton;
class UStatusComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReturnSelectionArtifact, bool, bSkipped, UStatusComponent*, Artifact);


/**
 * UUW_ArtifactRewardScreen 클래스는 아티팩트 보상 화면을 제공하는 UI 위젯입니다.
 * 플레이어가 보상을 선택하거나 스킵할 수 있도록 버튼을 제공하며, 선택된 아티팩트의 정보를 UI에 표시합니다.
 * 주로 게임에서 보상 선택 화면으로 사용되며, 아티팩트에 대한 정보를 시각적으로 제공하고, 
 * 플레이어의 선택에 따라 아티팩트를 수락하거나 스킵하는 기능을 수행합니다.
 * 
 * 주요 기능:
 * - 아티팩트 선택 버튼과 스킵 버튼 제공
 * - 선택된 아티팩트 정보를 화면에 업데이트
 * - 선택 또는 스킵 시 Delegate를 통해 이벤트 전달
 */
UCLASS(Blueprintable, BlueprintType)
class CROWNOFSIN_API UUW_ArtifactRewardScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_ArtifactRewardScreen(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

protected:
	// 아티팩트 버튼 클릭 시 호출되는 함수
	UFUNCTION()
	void OnClicked_ArtifactButton();

	// 스킵 버튼 클릭 시 호출되는 함수
	UFUNCTION()
	void OnClicked_SkipButton();

public:
	// 아티팩트 보상을 업데이트하는 함수
	UFUNCTION(BlueprintCallable, Category = "ArtifactRewardScreen")
	void UpdateReward(UStatusComponent* InStatus);


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	// 아티팩트 선택 버튼
	UPROPERTY(BlueprintReadWrite, Category="UW Artifact Reward Screen", meta = (BindWidget))
	UButton* ArtifactButton;

	// 보상을 스킵하는 버튼
	UPROPERTY(BlueprintReadWrite, Category="UW Artifact Reward Screen", meta = (BindWidget))
	UButton* SkipButton;

	// 보상 화면의 타이틀 텍스트
	UPROPERTY(BlueprintReadWrite, Category="UW Artifact Reward Screen", meta = (BindWidget))
	UTextBlock* Title;

	// 아티팩트 상태를 표시하는 위젯
	UPROPERTY(BlueprintReadWrite, Category="UW Artifact Reward Screen", meta = (BindWidget))
	UUW_StatusIcon* WBP_StatusIcon;

	// 선택된 아티팩트를 나타내는 컴포넌트
	UPROPERTY(BlueprintReadWrite, Category = "UW Artifact Reward Screen")
	UStatusComponent* Artifact;

	// 화면에 표시될 때 실행되는 페이드인 애니메이션
	UPROPERTY(BlueprintReadWrite, Category="UW Artifact Reward Screen", Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeIn;

public:
	// 아티팩트 선택 또는 스킵 이벤트를 전달하는 Delegate
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "UW Artifact Reward Screen|Delegate")
	FOnReturnSelectionArtifact OnReturnSelectionArtifact;
};
