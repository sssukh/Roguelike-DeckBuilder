#include "UI/UW_StoryButton.h"

#include "Components/Button.h"
#include "UI/UW_ToolTip.h"
#include "Utilities/CosLog.h"

UUW_StoryButton::UUW_StoryButton(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), StoryButton(nullptr), SpawnedCard(nullptr)
{
}

void UUW_StoryButton::NativeConstruct()
{
	Super::NativeConstruct();

	StoryButton->OnClicked.AddDynamic(this, &UUW_StoryButton::OnClicked_StoryButton);

	if (!WBPToolTipClass)
	{
		COS_SCREEN(TEXT("WBPToolTipClass 클래스를 설정해주세요!!"));
		return;
	}

	if (EncounterCard.CardEffects.IsEmpty()) return;
	
	FTransform SpawnTransform = FTransform::Identity;
	if (ACardBase* NewCard = GetWorld()->SpawnActorDeferred<ACardBase>(ACardBase::StaticClass(), SpawnTransform,
	                                                                   nullptr, nullptr,
	                                                                   ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
	{
		NewCard->CardDataDeck = EncounterCard;
		NewCard->FinishSpawning(SpawnTransform);

		SpawnedCard = NewCard;

		if (!SpawnedCard->GetCardDescription(ECardDataType::Hand).IsEmpty())
		{
			UUW_ToolTip* NewToolTipWidget = CreateWidget<UUW_ToolTip>(GetOwningPlayer(), WBPToolTipClass);
			SetToolTip(NewToolTipWidget);
		}

		RefreshPlayability();
	}
}

void UUW_StoryButton::OnClicked_StoryButton()
{
	if (IsValid(SpawnedCard))
	{
		TArray<AActor*> Targets;
		SpawnedCard->AttemptUseCard(Targets, false, false, false);
	}
	else
	{
		if (OnEmptyButtonClicked.IsBound())
			OnEmptyButtonClicked.Broadcast(this);
	}
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
