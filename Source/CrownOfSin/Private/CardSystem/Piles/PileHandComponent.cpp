#include "CardSystem/Piles/PileHandComponent.h"

#include "Libraries/FunctionLibrary_Event.h"
#include "Utilities/CosGameplayTags.h"
#include "CardSystem/CardBase.h"
#include "CardSystem/Piles/PileDrawComponent.h"
#include "Core/DispatcherHubLocalComponent.h"
#include "Libraries/FunctionLibrary_ArrayUtils.h"


UPileHandComponent::UPileHandComponent(): DrawnCard(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MaxPileSize = 10;
	FriendlyName = FText::FromString(FString(TEXT("Hand")));
	PileTag = CosGameTags::Pile_Hand;
}

void UPileHandComponent::BeginPlay()
{
	Super::BeginPlay();

	FGameplayTagContainer EventTags;
	EventTags.AddTag(CosGameTags::Event_Card_ModifyInHand);
	UFunctionLibrary_Event::BindMultipleEventsToGlobalDispatcherHub(this, EventTags);
}

void UPileHandComponent::AttemptDraw()
{
	// 손에 든 카드의 수가 더미의 최대 허용 크기보다 작은지 확인합니다.
	if (MaxPileSize > Cards.Num())
	{
		// 뽑은 카드를 저장할 포인터 변수를 선언합니다. 현재는 특정 카드를 지정하지 않으므로 nullptr을 전달합니다.
		ACardBase* NewlyDrawnCard = nullptr;

		// 카드 더미에서 새로운 카드를 뽑기 위해 DrawCard 함수를 호출합니다.
		// nullptr을 전달하여 특정 카드를 요청하지 않으며, 카드는 UPileDrawComponent 클래스에서 뽑힙니다.
		DrawCard(nullptr, UPileDrawComponent::StaticClass(), NewlyDrawnCard);
	}
}

bool UPileHandComponent::DrawCard(ACardBase* SpecifiedCard, TSubclassOf<UPileHandComponent> SourcePile, ACardBase*& OutDrawnCard)
{
	// SourcePile로부터 컴포넌트를 가져와서 캐스팅합니다. 성공하지 못하면 더미가 유효하지 않으므로 false를 반환합니다.
	UPileComponent* SourcePileComponent = Cast<UPileComponent>(GetOwner()->GetComponentByClass(SourcePile));
	if (!SourcePileComponent)
	{
		// 더미가 유효하지 않으면 nullptr을 반환하고 함수 종료
		OutDrawnCard = nullptr;
		return false;
	}

	// 카드 더미가 비어있는지 확인합니다.
	if (SourcePileComponent->Cards.Num() <= 0)
	{
		// 더미가 UPileDrawComponent의 자식 클래스가 아닌 경우 바로 실패를 반환합니다.
		if (!SourcePile->IsChildOf(UPileDrawComponent::StaticClass()))
		{
			OutDrawnCard = nullptr;
			return false;
		}

		// 더미가 비었으므로 카드 뽑기 컴포넌트에서 리셔플을 시도합니다.
		UPileDrawComponent* PileDrawComponent = Cast<UPileDrawComponent>(GetOwner()->GetComponentByClass(UPileDrawComponent::StaticClass()));
		if (!PileDrawComponent)
		{
			// PileDrawComponent가 없을 경우 nullptr을 반환하고 종료
			OutDrawnCard = nullptr;
			return false;
		}

		// 리셔플을 수행하여 버린 카드들을 다시 섞습니다.
		PileDrawComponent->ReshuffleDiscardIntoDraw();

		// 리셔플 후에도 카드가 없으면 false를 반환합니다.
		if (SourcePileComponent->Cards.Num() <= 0)
		{
			OutDrawnCard = nullptr;
			return false;
		}
	}

	// 만약 특정 카드를 지정했다면 해당 카드를 더미에서 찾습니다.
	if (IsValid(SpecifiedCard))
	{
		// 지정된 카드가 더미 안에 있는지 확인하고, 존재하면 그 카드를 선택합니다.
		int32 CardIndex = SourcePileComponent->Cards.Find(SpecifiedCard);
		if (CardIndex != INDEX_NONE)
		{
			DrawnCard = SpecifiedCard; // 요청한 카드를 찾으면 해당 카드를 선택
		}
		else
		{
			// 카드를 찾지 못했을 경우 무작위로 선택하거나 첫 번째 카드를 선택합니다.
			int32 RandomIndex;
			DrawnCard = bRandomDrawOrder ? UFunctionLibrary_ArrayUtils::GetRandomElementFromArray(SourcePileComponent->Cards, RandomIndex) : SourcePileComponent->Cards[0];
		}
	}
	else
	{
		// 특정 카드를 지정하지 않았을 경우, 무작위로 카드를 선택하거나 첫 번째 카드를 선택합니다.
		int32 RandomIndex;
		DrawnCard = bRandomDrawOrder ? UFunctionLibrary_ArrayUtils::GetRandomElementFromArray(SourcePileComponent->Cards, RandomIndex) : SourcePileComponent->Cards[0];
	}

	// 선택된 카드에 대해 이벤트를 호출하여 해당 카드가 뽑혔음을 알립니다.
	OutDrawnCard = DrawnCard;
	return DrawnCard->DispatcherHubLocal->CallEvent(CosGameTags::Event_Card_Draw, OutDrawnCard);
}

void UPileHandComponent::DiscardHand()
{
	// Cards 배열을 로컬 변수로 복사하여 안전하게 반복 작업을 진행합니다.
	TArray<ACardBase*> HandCards = Cards;

	// 손에 있는 모든 카드를 순회합니다.
	for (ACardBase* CardInHand : HandCards)
	{
		// 각 카드에 대해 턴 종료 시 발생하는 패시브 이벤트를 호출합니다.
		FGameplayTagContainer CallTags;
		CallTags.AddTag(CosGameTags::Flag_Passive);
		CardInHand->RunEndTurnEvent(CallTags);
	}
}

void UPileHandComponent::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	// EventTag가 카드 손 패 수정 이벤트인 경우만 처리합니다.
	if (EventTag == CosGameTags::Event_Card_ModifyInHand)
	{
		// CallingObject가 유효하고, ACardBase 타입으로 캐스팅이 성공하면 처리합니다.
		if (ACardBase* ModifiedCard = Cast<ACardBase>(CallingObject))
		{
			// 손에 있는 카드의 수정 사항을 초기화합니다.
			ModifiedCard->ResetInHandModifications();
		}
	}
}

bool UPileHandComponent::AttemptAddExistingCard_Implementation(ACardBase* InCard)
{
	// 기본 PileComponent의 AttemptAddExistingCard를 호출하여 카드 추가 시도
	if (Super::AttemptAddExistingCard_Implementation(InCard))
	{
		// 카드가 성공적으로 추가된 경우, 손 패에 들어갔다는 이벤트를 발생시킵니다.
		InCard->CallLocalEventOnCard(CosGameTags::Event_Card_EnterHand);
		return true;
	}

	// 카드 추가 실패 시 false 반환
	return false;
}

bool UPileHandComponent::RequestRemoveCardFromPile_Implementation(ACardBase* InCard)
{
	// 기본 PileComponent의 RequestRemoveCardFromPile을 호출하여 카드 제거 시도
	Super::RequestRemoveCardFromPile_Implementation(InCard);

	// 카드가 성공적으로 제거되었으면 손 패에서 나갔다는 이벤트를 호출합니다.
	InCard->CallLocalEventOnCard(CosGameTags::Event_Card_ExitHand);

	// 카드의 손 패에서의 수정 상태를 초기화합니다.
	InCard->ResetInHandModifications();

	return true;
}
