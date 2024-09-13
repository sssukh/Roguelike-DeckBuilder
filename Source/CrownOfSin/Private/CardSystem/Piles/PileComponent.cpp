#include "CardSystem/Piles/PileComponent.h"

#include "BlueprintGameplayTagLibrary.h"
#include "CardSystem/CardBase.h"
#include "CardSystem/CardPlayer.h"
#include "CardSystem/Piles/PileDestroyComponent.h"

#include "Core/DispatcherHubLocalComponent.h"
#include "Libraries/AssetTableRef.h"
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

	// ...
	FriendlyName = FText::FromString(FString(TEXT("UnNamed Pile")));
	PileTag = CosGameTags::Pile;

	if (UDataTable* DT_Tooltips_Statuses = FAssetReferenceUtility::LoadAssetFromDataTable<UDataTable>(AssetRefPath::DataTablePath, FName("DT_Tooltips_Statuses")))
	{
		Tooltip.DataTable = DT_Tooltips_Statuses;
		Tooltip.RowName = FName(TEXT("Deck"));
	}
}

void UPileComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UPileComponent::ResolveAddNewCardBeyondMax()
{
	COS_SCREEN(TEXT("경고: MaxDeckSize를 초과하는 카드를 추가하려고 시도했습니다."));
}

void UPileComponent::RemoveAllCardsOwnedByActor(AActor* InActor)
{
	// 더미에 있는 모든 카드를 순회합니다.
	for (ACardBase* Card : Cards)
	{
		// 입력된 액터가 이 컴포넌트를 소유한 액터(카드의 소유자)와 동일한지 확인합니다.
		if (InActor == GetOwner())
		{
			// 소유한 카드가 확인되면 해당 카드의 이벤트 디스패처에서 "무효화" 이벤트를 호출합니다.
			// 이벤트 태그는 'Event_Card_Void'로 설정되어 있습니다.
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
		if (Card->GetGameplayTags().HasAllExact(RequiredTags))
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
	// 전달된 카드 배열을 순회하면서 하나씩 더미에 추가합니다.
	for (ACardBase* Card : InDeck)
	{
		// 각 카드를 더미에 추가하려고 시도합니다.
		Execute_AttemptAddExistingCard(this, Card);
	}
}

bool UPileComponent::ClearPile_Implementation()
{
	// 현재 카드 플레이어를 가져옵니다. 만약 플레이어를 찾지 못하면 false를 반환합니다.
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	if (!CardPlayer)
	{
		return false; // 플레이어가 없을 경우 실패
	}

	// 카드 플레이어의 PileDestroyComponent에 현재 더미의 모든 카드를 전달합니다.
	Execute_AssignExistingCards(CardPlayer->PileDestroyComponent, Cards);

	// 더미의 모든 카드를 제거합니다.
	Cards.Empty();

	// 성공적으로 카드를 제거한 경우 true를 반환합니다.
	return true;
}

bool UPileComponent::RequestCardFromPile_Implementation(TSubclassOf<UPileComponent> RequestingPile, ACardBase* SpecificCard, bool bIsRandomCard,
                                                        const FGameplayTagContainer& RequiredTags, ACardBase*& OutCard)
{
	// 요청된 특정 카드가 유효한지 확인합니다.
	if (IsValid(SpecificCard))
	{
		// 요청된 특정 카드를 카드 목록에서 찾습니다.
		int32 CardIndex = Cards.Find(SpecificCard);
		if (CardIndex >= 0) // 찾은 경우
		{
			OutCard = SpecificCard; // 요청된 특정 카드를 반환합니다.
			return true;
		}

		// 특정 카드를 찾지 못한 경우
		OutCard = nullptr;
		return false;
	}

	// 카드 목록에 카드가 있는지 확인합니다.
	if (Cards.Num() > 0)
	{
		// 필수 태그가 있을 경우, 해당 태그를 가진 카드를 반환합니다.
		if (RequiredTags.Num() > 0)
		{
			return TryGetCardWithTags(RequiredTags, OutCard);
		}

		// 무작위 카드를 요청한 경우
		if (bIsRandomCard)
		{
			return GetRandomCard(OutCard);
		}

		// 첫 번째 카드를 반환합니다.
		OutCard = Cards[0];
		return true;
	}

	// 실패한 경우
	OutCard = nullptr;
	return false;
}

bool UPileComponent::RequestRemoveCardFromPile_Implementation(ACardBase* InCard)
{
	// 카드 목록에서 지정된 카드를 제거하고, 제거된 카드 수를 반환합니다.
	int32 RemovedCount = Cards.Remove(InCard);

	// 카드를 하나 이상 제거한 경우
	if (RemovedCount > 0)
	{
		// 카드의 현재 더미 상태를 None으로 설정합니다.
		InCard->CurrentPile = CosGameTags::None;
		return true;
	}

	// 제거하지 못한 경우 false 반환
	return false;
}

bool UPileComponent::AttemptAddExistingCard_Implementation(ACardBase* InCard)
{
	// 현재 더미에 저장된 카드 수가 최대 크기보다 적은지 확인합니다.
	if (Cards.Num() < MaxPileSize)
	{
		// 더미에 카드를 추가하고, 해당 카드의 현재 위치를 갱신합니다.
		Cards.Add(InCard);
		InCard->CurrentPile = PileTag; // 카드가 추가된 더미를 기록
		return true;
	}

	// 더미의 최대 크기를 초과한 경우 처리 함수 호출
	ResolveAddNewCardBeyondMax();
	return false;
}

bool UPileComponent::SpawnAndAddNewCard_Implementation(const FDataTableRowHandle& DataRowHandle, ACardBase*& OutCard)
{
	// 현재 더미의 카드 수가 최대 크기를 초과하지 않는지 확인합니다.
	if (Cards.Num() < MaxPileSize)
	{
		// 소유자의 위치와 회전 정보를 가져옵니다.
		FTransform SpawnTransform = GetOwner()->GetActorTransform();

		// 카드를 지연 생성합니다.
		if (ACardBase* NewCard = GetWorld()->SpawnActorDeferred<ACardBase>(
			ACardBase::StaticClass(), // 생성할 카드의 클래스입니다.
			SpawnTransform, // 생성할 위치와 회전 정보입니다.
			nullptr, // 소유자는 없으므로 nullptr 설정
			nullptr, // 인스턴스의 템플릿은 nullptr로 설정
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn)) // 충돌 처리 방식 설정
		{
			// 데이터 테이블에서 해당 행의 카드 데이터를 찾습니다.
			if (FCard* FoundCard = DataRowHandle.DataTable->FindRow<FCard>(DataRowHandle.RowName, TEXT("")))
			{
				NewCard->CardDataDeck = *FoundCard; // 생성된 카드에 데이터 적용
			}

			// 카드를 완전히 생성합니다.
			NewCard->FinishSpawning(SpawnTransform);

			// 생성된 카드를 출력 변수에 저장합니다.
			OutCard = NewCard;

			// 카드를 더미에 추가하고 결과를 반환합니다.
			return Execute_AttemptAddExistingCard(this, NewCard);
		}
	}

	// 더미의 최대 크기를 초과한 경우 경고를 출력합니다.
	ResolveAddNewCardBeyondMax();
	return false;
}

TArray<ACardBase*> UPileComponent::GetCards_Implementation()
{
	return Cards;
}
