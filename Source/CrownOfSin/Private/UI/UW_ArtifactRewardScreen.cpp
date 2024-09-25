#include "UI/UW_ArtifactRewardScreen.h"

#include "Components/Button.h"
#include "UI/UW_StatusIcon.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"

UUW_ArtifactRewardScreen::UUW_ArtifactRewardScreen(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), ArtifactButton(nullptr), SkipButton(nullptr), Title(nullptr),
                                                                                                  WBP_StatusIcon(nullptr),
                                                                                                  Artifact(nullptr), FadeIn(nullptr)
{
}

void UUW_ArtifactRewardScreen::NativeConstruct()
{
	Super::NativeConstruct();
	ArtifactButton->OnClicked.AddDynamic(this, &UUW_ArtifactRewardScreen::OnClicked_ArtifactButton);
	SkipButton->OnClicked.AddDynamic(this, &UUW_ArtifactRewardScreen::OnClicked_SkipButton);
}

void UUW_ArtifactRewardScreen::OnClicked_ArtifactButton()
{
	if (OnReturnSelectionArtifact.IsBound())
		OnReturnSelectionArtifact.Broadcast(false, Artifact);

	PlayAnimationReverse(FadeIn);
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UUW_ArtifactRewardScreen::OnClicked_SkipButton()
{
	if (OnReturnSelectionArtifact.IsBound())
		OnReturnSelectionArtifact.Broadcast(true, nullptr);

	PlayAnimationReverse(FadeIn);
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UUW_ArtifactRewardScreen::UpdateReward(UStatusComponent* InStatus)
{
	Artifact = InStatus;

	COS_IF_CHECK(WBP_StatusIcon->GetClass()->ImplementsInterface(UInterface_StatusIcon::StaticClass()), TEXT("WBP_StatusIcon이 Interface_StatusIcon을 상속받지 않았습니다."));

	IInterface_StatusIcon::Execute_InitStatusWidget(WBP_StatusIcon, Artifact, CosGameTags::StatusBar_RewardBar, true);

	SetVisibility(ESlateVisibility::Visible);

	PlayAnimation(FadeIn);
}
