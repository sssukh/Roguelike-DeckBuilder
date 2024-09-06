#include "UI/UW_StoryButton.h"

#include "Components/Button.h"

UUW_StoryButton::UUW_StoryButton(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), StoryButton(nullptr), SpawnedCard(nullptr)
{
}

void UUW_StoryButton::RefreshPlayability()
{
	if (IsValid(SpawnedCard))
	{
		FString FailMessage;
		bool bPlayable = SpawnedCard->CheckIfPlayable(FailMessage);
		StoryButton->SetIsEnabled(bPlayable);
	}
}
