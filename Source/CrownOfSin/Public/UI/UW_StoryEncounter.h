#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/CosEnumStruct.h"
#include "Interfaces/Interface_EventHolder.h"
#include "Interfaces/Interface_StoryEncounter.h"
#include "UW_StoryEncounter.generated.h"

class UUW_StoryButton;
class URichTextBlock;
class UVerticalBox;
class UImage;

/**
 * UUW_StoryEncounter 클래스는 게임 내 스토리 이벤트를 처리하는 UI 위젯입니다.
 * 스토리 이벤트와 관련된 선택지(옵션)들을 표시하고, 플레이어가 선택할 수 있는 버튼을 제공합니다.
 * 각 스토리 이벤트는 설명 텍스트, 이미지, 선택지 카드들로 구성됩니다.
 * 이 클래스는 IInterface_EventHolder와 IInterface_StoryEncounter를 상속받아 이벤트 처리와 스토리 관련 기능을 구현합니다.
 * 
 * 주요 기능:
 * - 스토리 이벤트 초기화 및 설정
 * - 선택지 버튼을 생성하고 화면에 표시
 * - 선택지 선택 시 처리 및 스토리 이벤트 제거
 */
UCLASS()
class CROWNOFSIN_API UUW_StoryEncounter : public UUserWidget, public IInterface_EventHolder, public IInterface_StoryEncounter
{
	GENERATED_BODY()

public:
	UUW_StoryEncounter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void SetupStoryEncounter(const FText& InDescription, UTexture2D* Image, const TArray<FCard>& EncounterOptions, bool bIsFirstScreen);

	UFUNCTION(BlueprintCallable)
	void BindRemovalToEmptyButton(UUW_StoryButton* StoryButton);

	UFUNCTION(BlueprintCallable)
	void DisableOptions();

	UFUNCTION(BlueprintCallable)
	void RemoveOnEmptyButtonClicked(UUserWidget* CallingWidget);

	virtual void NativeDestruct() override;

	/*========================================================================================
	*	Interface_Event Holder
	=========================================================================================*/
public:
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;

	/*========================================================================================
	*	Interface_StoryEncounter
	=========================================================================================*/
public:
	virtual void InitializeStoryEncounter_Implementation(FDataTableRowHandle EncounterData, bool bIsFirstScreen) override;


	/*========================================================================================
	 *	Field Members
	 =========================================================================================*/
public:
	UPROPERTY(BlueprintReadWrite, Category = "Story Encounter Widget|Designer", meta=(BindWidget))
	TObjectPtr<UImage> Background;

	UPROPERTY(BlueprintReadWrite, Category = "Story Encounter Widget|Designer", meta=(BindWidget))
	TObjectPtr<UVerticalBox> ButtonBox;

	UPROPERTY(BlueprintReadWrite, Category = "Story Encounter Widget|Designer", meta=(BindWidget))
	TObjectPtr<URichTextBlock> Description;

	UPROPERTY(BlueprintReadWrite, Category = "Story Encounter Widget|Designer", meta=(BindWidget))
	TObjectPtr<UVerticalBox> StoryBox;

	UPROPERTY(BlueprintReadWrite, Category = "Story Encounter Widget|Designer", meta=(BindWidget))
	TArray<UUW_StoryButton*> StoryButtons;

	UPROPERTY(BlueprintReadWrite, Transient, meta=(BindWidgetAnim), Category="Story Encounter Widget|Designer")
	TObjectPtr<UWidgetAnimation> StoryBoxFadeIn;

	UPROPERTY(BlueprintReadWrite, Transient, meta=(BindWidgetAnim), Category="Story Encounter Widget|Designer")
	TObjectPtr<UWidgetAnimation> FadeIn;

public:
	/** 
	 * 스토리 버튼 클래스
	 * 스토리 선택지를 생성하는 데 사용됩니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story Encounter Widget|Config|Setup")
	TSubclassOf<UUW_StoryButton> StoryButtonClass;
};
