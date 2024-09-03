#include "UI/UW_RewardScreen.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "UI/UW_StatusBar.h"
#include "Utilities/CosGameplayTags.h"

UUW_RewardScreen::UUW_RewardScreen(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
                                                                                  TakeAllButton(nullptr), SkipButton(nullptr),
                                                                                  ButtonText(nullptr), WBP_ArtifactBar(nullptr), FadeIn(nullptr)
{
}

void UUW_RewardScreen::NativeConstruct()
{
	Super::NativeConstruct();

	TakeAllButton->OnClicked.AddDynamic(this, &UUW_RewardScreen::OnClicked_TakeAllButton);
	SkipButton->OnClicked.AddDynamic(this, &UUW_RewardScreen::OnClicked_SkipButton);
}

void UUW_RewardScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (GetVisibility() == ESlateVisibility::Visible)
	{
		int32 VisibleRewardCount;
		bool bSuccess = WBP_ArtifactBar->GetNumberOfVisibleRewards(VisibleRewardCount);

		if (bSuccess && VisibleRewardCount == 0)
		{
			if (bCloseWhenEmpty)
			{
				OnClicked_SkipButton();
			}
			else
			{
				ButtonText->SetText(FText::FromString(TEXT("Continue")));
			}
		}
	}
}

void UUW_RewardScreen::Appear()
{
	SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(FadeIn);
}

void UUW_RewardScreen::OnClicked_TakeAllButton()
{
	WBP_ArtifactBar->InteractWithAllStatuses();
}

void UUW_RewardScreen::OnClicked_SkipButton()
{
	PlayAnimationReverse(FadeIn);
	SetVisibility(ESlateVisibility::HitTestInvisible);
	UFunctionLibrary_Event::CallEventInGlobalDispatcherHub(CosGameTags::Event_CloseRewardScreen, this, nullptr);
	WBP_ArtifactBar->ClearAllStatuses();
}
