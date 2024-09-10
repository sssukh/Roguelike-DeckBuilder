#include "CardSystem/Piles/PileComponent.h"

#include "BlueprintGameplayTagLibrary.h"
#include "CardSystem/CardBase.h"
#include "CardSystem/CardPlayer.h"
#include "CardSystem/Piles/PileDestroyComponent.h"

#include "Core/DispatcherHubLocalComponent.h"
#include "Libraries/FunctionLibrary_ArrayUtils.h"
#include "Libraries/FunctionLibrary_Singletons.h"

#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


UPileComponent::UPileComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;


	FriendlyName = FText::FromString(FString(TEXT("UnNamed Pile")));

	PileTag = CosGameTags::Pile;
	// ...
}

void UPileComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UPileComponent::ResolveAddNewCardBeyondMax()
{
	COS_LOG_SCREEN(TEXT("경고: MaxDeckSize를 초과하는 카드를 추가하려고 시도했습니다."));
}

void UPileComponent::RemoveAllCardsOwnedByActor(AActor* InActor)
{
	for (ACardBase* Card : Cards)
	{
		if (InActor == GetOwner())
		{
			Card->DispatcherHubLocal->CallEvent(CosGameTags::Event_Card_Void, Card);
		}
	}
}

int32 UPileComponent::GetCardCount()
{
	return Cards.Num();
}

bool UPileComponent::TryGetCardWithTags(const FGameplayTagContainer& RequiredTags, ACardBase*& OutCard)
{
	for (ACardBase* Card : Cards)
	{
		if (UBlueprintGameplayTagLibrary::HasAllTags(Card->GetGameplayTags(), RequiredTags, true))
		{
			OutCard = Card;
			return true;
		}
	}

	OutCard = nullptr;
	return false;
}

bool UPileComponent::GetRandomCard(ACardBase*& OutCard)
{
	int32 RandomIndex;
	OutCard = UFunctionLibrary_ArrayUtils::GetRandomElementFromArray<ACardBase*>(Cards, RandomIndex);
	return OutCard != nullptr;
}

void UPileComponent::AssignExistingCards_Implementation(const TArray<ACardBase*>& InDeck)
{
	for (ACardBase* Card : InDeck)
	{
		Execute_AttemptAddExistingCard(this, Card);
	}
}

bool UPileComponent::ClearPile_Implementation()
{
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	UPileDestroyComponent* PileDestroyComponent = CardPlayer->PileDestroyComponent;
	Execute_AssignExistingCards(PileDestroyComponent, Cards);
	Cards.Empty();
	return true;
}

bool UPileComponent::RequestCardFromPile_Implementation(TSubclassOf<UPileComponent> RequestingPile, ACardBase* SpecificCard, bool bIsRandomCard,
                                                        const FGameplayTagContainer& RequiredTags, ACardBase*& OutCard)
{
	// 특정 카드를 요청한 경우
	if (IsValid(SpecificCard))
	{
		int32 FindIndex = Cards.Find(SpecificCard);
		if (FindIndex >= 0)
		{
			OutCard = SpecificCard;
			return true;
		}

		OutCard = nullptr;
		return false;
	}

	// 덱이 비어있지 않으면
	if (Cards.Num() > 0)
	{
		// 태그 조건이 있는 경우
		if (RequiredTags.Num() > 0)
		{
			 return TryGetCardWithTags(RequiredTags, OutCard);
		}

		// 무작위 카드 요청인 경우
		if (bIsRandomCard)
		{
			return GetRandomCard(OutCard);
		}

		// 첫 번째 카드 반환
		OutCard = Cards[0];
		return true;
	}

	// 실패한 경우
	OutCard = nullptr;
	return false;
}

bool UPileComponent::RequestRemoveCardFromPile_Implementation(ACardBase* InCard)
{
	int32 RemoveCount = Cards.Remove(InCard);

	if (RemoveCount > 0)
	{
		InCard->CurrentPile = CosGameTags::None;
		return true;
	}

	return false;
}

bool UPileComponent::AttemptAddExistingCard_Implementation(ACardBase* InCard)
{
	if (Cards.Num() < MaxPileSize)
	{
		Cards.Add(InCard);
		InCard->CurrentPile = PileTag;
		return true;
	}

	ResolveAddNewCardBeyondMax();
	return false;
}

bool UPileComponent::SpawnAndAddNewCard_Implementation(const FDataTableRowHandle& DataRowHandle, ACardBase*& OutCard)
{
	if (Cards.Num() < MaxPileSize)
	{
		FTransform SpawnTransform = GetOwner()->GetActorTransform();

		if (ACardBase* NewCard = GetWorld()->SpawnActorDeferred<ACardBase>(ACardBase::StaticClass(), SpawnTransform,
		                                                                   nullptr, nullptr,
		                                                                   ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
		{
			if (FCard* FoundCard = DataRowHandle.DataTable->FindRow<FCard>(DataRowHandle.RowName,TEXT("")))
				NewCard->CardDataDeck = *FoundCard;
			NewCard->FinishSpawning(SpawnTransform);

			OutCard = NewCard;
			return Execute_AttemptAddExistingCard(this, NewCard);
		}
	}

	ResolveAddNewCardBeyondMax();
	return false;
}

TArray<ACardBase*> UPileComponent::GetCards_Implementation()
{
	return Cards;
}
