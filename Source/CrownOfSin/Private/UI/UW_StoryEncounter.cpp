#include "UI/UW_StoryEncounter.h"

#include "Animation/WidgetAnimation.h"
#include "UI/UW_StoryButton.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "Components/VerticalBox.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"

UUW_StoryEncounter::UUW_StoryEncounter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUW_StoryEncounter::SetupStoryEncounter(const FText& InDescription, UTexture2D* Image, TArray<FCard> EncounterOptions,
                                             bool bIsFirstScreen)
{
	Description->SetText(InDescription);

	Background->SetBrushFromTexture(Image, false);

	// 마진 패딩 설정값 미리 지정해둠.
	FMargin ButtonPadding(0.0f, 12.0f);

	if (!StoryButtonClass)
	{
		COS_SCREEN(TEXT("StoryButtonClass 클래스를 설정해주세요!!!"));
		return;
	}
	for (const FCard& Option : EncounterOptions)
	{
		UUW_StoryButton* NewStoryButtonWidget = CreateWidget<UUW_StoryButton>(this, StoryButtonClass);

		NewStoryButtonWidget->EncounterCard = Option;

		NewStoryButtonWidget->SetPadding(ButtonPadding);

		ButtonBox->AddChild(NewStoryButtonWidget);

		StoryButtons.Add(NewStoryButtonWidget);

		BindRemovalToEmptyButton(NewStoryButtonWidget);
	}

	PlayAnimation(StoryBoxFadeIn);

	if (!bIsFirstScreen)
	{
		return;
	}

	PlayAnimation(FadeIn);
}

void UUW_StoryEncounter::BindRemovalToEmptyButton(UUW_StoryButton* StoryButton)
{
	StoryButton->OnEmptyButtonClicked.AddDynamic(this, &UUW_StoryEncounter::RemoveOnEmptyButtonClicked);
}

void UUW_StoryEncounter::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	for (UUW_StoryButton* Button : StoryButtons)
	{
		Button->RefreshPlayability();
	}
}

void UUW_StoryEncounter::InitializeStoryEncounter_Implementation(FDataTableRowHandle EncounterData, bool bIsFirstScreen)
{
	UFunctionLibrary_Event::BindEventToGlobalDispatcherHub(this, CosGameTags::Event_PostModifyStatus);

	if (EncounterData.IsNull())
	{
		COS_SCREEN(TEXT("EncounterData가 존재하지 않습니다. in %s"), *GetNameSafe(this));
		return;
	}

	FStoryEncounter StoryEncounter = *EncounterData.DataTable->FindRow<FStoryEncounter>(EncounterData.RowName,TEXT("FStoryEncounter in UW_StoryEncounter"));

	SetupStoryEncounter(StoryEncounter.Description, StoryEncounter.Image, StoryEncounter.EncounterOptions, bIsFirstScreen);
}

void UUW_StoryEncounter::DisableOptions()
{
	ButtonBox->SetIsEnabled(false);
}

void UUW_StoryEncounter::RemoveOnEmptyButtonClicked(UUserWidget* CallingWidget)
{
	if (!GetGameInstance()->GetClass()->ImplementsInterface(UInterface_CardGameInstance::StaticClass()))
	{
		COS_SCREEN(TEXT("게임 인스턴스가 UInterface_CardGameInstance를 상속받지 않았습니다"));
		return;
	}

	IInterface_CardGameInstance::Execute_AttemptSaveGame(GetGameInstance(),TEXT(""), true);

	RemoveFromParent();
}

void UUW_StoryEncounter::NativeDestruct()
{
	Super::NativeDestruct();
	UFunctionLibrary_Event::UnBindEventFromGlobalDispatcherHub(this, CosGameTags::Event_PostModifyStatus);
}
