#include "UI/UW_Layout_Cos.h"

#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/WidgetSwitcher.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UW_RewardScreen.h"
#include "UI/UW_ScreenFade.h"


UUW_Layout_Cos::UUW_Layout_Cos(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
                                                                              DeckListSwitcher(nullptr), StoryEncounterBox(nullptr), WBP_RewardScreen(nullptr)
{
}

void UUW_Layout_Cos::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAnimationReverse(WBP_ScreenFade->Occlude, 0.5f);

	SetupDebugMode();

	UpdateHandAnchorWidgets();

	// SetupPileWidgetMap();

	UpdateHandAnchorWidgets();
}

void UUW_Layout_Cos::SetupDebugMode()
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	if (GameInstance->GetClass()->ImplementsInterface(UInterface_CardGameInstance::StaticClass()))
	{
		bool bIsDebugMode = IInterface_CardGameInstance::Execute_IsDebugMode(GameInstance);
		ESlateVisibility NewVisibility = bIsDebugMode ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
		DebugButton->SetVisibility(NewVisibility);
	}
}

void UUW_Layout_Cos::UpdateHandAnchorWidgets()
{
	
}

bool UUW_Layout_Cos::ShouldNodeMapBeBlocked()
{
	return WBP_RewardScreen->GetVisibility() == ESlateVisibility::Visible ||
		StoryEncounterBox->GetChildrenCount() > 0 ||
		DeckListSwitcher->IsVisible();
}
