#include "CardSystem/CardTransferComponent.h"

#include "ActionSystem/ActionManagerSubsystem.h"
#include "ActionSystem/Action_CardTransfer.h"
#include "ActionSystem/Action_Reshuffle.h"
#include "CardSystem/CardBase.h"
#include "CardSystem/CardPlayer.h"
#include "CardSystem/Piles/PileDiscardComponent.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"

UCardTransferComponent::UCardTransferComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}

void UCardTransferComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	// 카드 관련 이벤트 태그 설정
	FGameplayTagContainer EventTags;
	EventTags.AddTag(CosGameTags::Event_Card_Destroy);
	EventTags.AddTag(CosGameTags::Event_Card_Discard);
	EventTags.AddTag(CosGameTags::Event_Card_Draw);
	EventTags.AddTag(CosGameTags::Event_Card_Exhaust);
	EventTags.AddTag(CosGameTags::Event_Card_GenerateCard);
	EventTags.AddTag(CosGameTags::Event_Card_PrePlay);
	EventTags.AddTag(CosGameTags::Event_Card_ReturnToHand);
	EventTags.AddTag(CosGameTags::Event_Card_Void);
	EventTags.AddTag(CosGameTags::Event_Reshuffle);

	// 이벤트 태그를 글로벌 디스패처에 바인딩
	UFunctionLibrary_Event::BindMultipleEventsToGlobalDispatcherHub(this, EventTags);
}

void UCardTransferComponent::Reshuffle()
{
	// 리셔플 이벤트 큐에 추가
	UFunctionLibrary_Event::QueueEventInGlobalDispatcherHub(CosGameTags::Event_Action_Reshuffle, this);

	// 액션 매니저 서브시스템을 통해 리셔플 액션을 생성하고 큐에 추가
	UActionManagerSubsystem* ActionManagerSubsystem = GetWorld()->GetSubsystem<UActionManagerSubsystem>();
	ActionManagerSubsystem->CreateAndQueueAction<AAction_Reshuffle>([&](AAction_Reshuffle* NewReShuffleAction)
	{
		ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
		NewReShuffleAction->Cards = CardPlayer->PileDiscardComponent->Cards;
		NewReShuffleAction->EndDelay = 0.2f;
	});
}

void UCardTransferComponent::RunCardTransferEvent(FGameplayTag EventTag, ACardBase* Card, FGameplayTagContainer CallTags)
{
	if (bInterrupt)
	{
		bInterrupt = false;
		return;
	}

	// 타겟 더미 설정 및 이벤트 처리
	auto HandleCardTransfer = [&](FGameplayTag TargetPile)
	{
		CurrentTargetPile = TargetPile;
		ResolveTransfer(Card, Card->CurrentPile, CurrentTargetPile);
	};

	// 카드 관련 이벤트 처리
	if (EventTag == CosGameTags::Event_Card_Discard)
	{
		CurrentTargetPile = CosGameTags::Pile_Discard;
		for (const FGameplayTag& CallTag : CallTags.GetGameplayTagArray())
		{
			if (CallTag == CosGameTags::Flag_Active)
			{
				UFunctionLibrary_Event::QueueEventInGlobalDispatcherHub(CosGameTags::Event_Action_FlickCard, Card, nullptr, 0.2f);
			}
		}

		ResolveTransfer(Card, Card->CurrentPile, CurrentTargetPile);
	}
	else if (EventTag == CosGameTags::Event_Card_Destroy)
	{
		HandleCardTransfer(CosGameTags::Pile_Destroy);
	}
	else if (EventTag == CosGameTags::Event_Card_Draw)
	{
		HandleCardTransfer(CosGameTags::Pile_Hand);
	}
	else if (EventTag == CosGameTags::Event_Card_Exhaust)
	{
		HandleCardTransfer(CosGameTags::Pile_Exhaust);
	}
	else if (EventTag == CosGameTags::Event_Card_Void)
	{
		HandleCardTransfer(CosGameTags::Pile_Void);
	}
	else if (EventTag == CosGameTags::Event_Card_GenerateCard)
	{
		float DisplayTime = CallTags.HasTag(CosGameTags::Flag_Slow) ? 1.5f : 0.4f;
		GenerateCard(Card, DisplayTime);
	}
	else if (EventTag == CosGameTags::Event_Card_PrePlay)
	{
		if (UPileHandComponent* PileHandComponent = GetOwner()->FindComponentByClass<UPileHandComponent>())
		{
			PileHandComponent->Cards.Remove(Card);
		}
	}
	else if (EventTag == CosGameTags::Event_Card_ReturnToHand)
	{
		ReturnCardToHand(Card, Card);
	}
	else if (EventTag == CosGameTags::Event_Reshuffle)
	{
		Reshuffle();
	}
}

bool UCardTransferComponent::ResolveTransfer(ACardBase* Card, FGameplayTag SourcePileTag, FGameplayTag TargetPileTag)
{
	// 유효성 검사: 카드 플레이어와 카드가 유효한지 체크
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	if (!CardPlayer || !Card)
	{
		COS_LOG_ERROR(TEXT("카드 플레이어 또는 카드가 유효하지 않습니다."));
		return false;
	}

	// 소스 더미에서 카드 제거
	UPileComponent* SourcePileComponent = nullptr;
	if (CardPlayer->GetPileWithPileTag(SourcePileTag, SourcePileComponent))
	{
		IInterface_Pile::Execute_RequestRemoveCardFromPile(SourcePileComponent, Card);
	}

	// 타겟 더미로 카드 추가
	UPileComponent* TargetPileComponent = nullptr;
	if (!CardPlayer->GetPileWithPileTag(TargetPileTag, TargetPileComponent))
	{
		COS_LOG_ERROR(TEXT("타겟 더미를 찾을 수 없습니다."));
		return false;
	}

	// 카드 추가가 성공적으로 이루어졌는지 확인
	IInterface_Pile::Execute_AttemptAddExistingCard(TargetPileComponent, Card);


	// 카드 전송 애니메이션 및 액션 처리
	UActionManagerSubsystem* ActionManagerSubsystem = GetWorld()->GetSubsystem<UActionManagerSubsystem>();
	ActionManagerSubsystem->CreateAndQueueAction<AAction_CardTransfer>([=](AAction_CardTransfer* NewCardTransferAction)
	{
		NewCardTransferAction->Card = Card;
		NewCardTransferAction->SourcePile = SourcePileTag;
		NewCardTransferAction->TargetPile = TargetPileTag;
		NewCardTransferAction->EndDelay = 0.1f;
	});

	return true;
}

void UCardTransferComponent::ReturnCardToHand(ACardBase* Card, UObject* CallingObject)
{
	// 유효성 검사: 카드 플레이어와 카드가 유효한지 체크
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	if (!CardPlayer || !Card)
	{
		COS_LOG_ERROR(TEXT("카드 플레이어 또는 카드가 유효하지 않습니다."));
		return;
	}

	// 손 패에 카드를 추가
	IInterface_Pile::Execute_AttemptAddExistingCard(CardPlayer->PileHandComponent, Card);

	// 카드가 손으로 돌아갔음을 알리는 이벤트 큐잉
	UFunctionLibrary_Event::QueueEventInGlobalDispatcherHub(CosGameTags::Event_Action_ReturnCardToHand, Card, nullptr, 0.1f);
}

void UCardTransferComponent::GenerateCard(ACardBase* Card, float DisplayTime)
{
	// 유효성 검사: 카드가 유효한지, 현재 더미 태그가 올바른지 확인
	if (!Card)
	{
		COS_LOG_ERROR(TEXT("생성할 카드가 유효하지 않습니다."));
		return;
	}

	// 더미 태그와 매칭되는지 확인하기 위한 태그 컨테이너 (클래스 내 상수로 설정하는 것을 고려)
	static FGameplayTagContainer LocalPileTagsToAnimate;
	LocalPileTagsToAnimate.AddTag(CosGameTags::Pile_Deck);
	LocalPileTagsToAnimate.AddTag(CosGameTags::Pile_Destroy);
	LocalPileTagsToAnimate.AddTag(CosGameTags::Pile_Discard);
	LocalPileTagsToAnimate.AddTag(CosGameTags::Pile_Draw);
	LocalPileTagsToAnimate.AddTag(CosGameTags::Pile_Exhaust);
	LocalPileTagsToAnimate.AddTag(CosGameTags::Pile_Hand);
	LocalPileTagsToAnimate.AddTag(CosGameTags::Pile_Void);

	// 카드의 현재 더미 태그가 애니메이션 처리 대상인지 확인
	if (Card->CurrentPile.MatchesAnyExact(LocalPileTagsToAnimate))
	{
		// 카드 생성 이벤트를 글로벌 디스패처에 큐잉
		UFunctionLibrary_Event::QueueEventInGlobalDispatcherHub(CosGameTags::Event_Action_GenerateCard, Card, nullptr, DisplayTime);

		// 액션 매니저 서브시스템을 통해 카드 전송 액션을 생성 및 큐잉
		UActionManagerSubsystem* ActionManagerSubsystem = GetWorld()->GetSubsystem<UActionManagerSubsystem>();
		ActionManagerSubsystem->CreateAndQueueAction<AAction_CardTransfer>([&](AAction_CardTransfer* NewCardTransferAction)
		{
			NewCardTransferAction->Card = Card;
			NewCardTransferAction->SourcePile = FGameplayTag();
			NewCardTransferAction->TargetPile = Card->CurrentPile;
			NewCardTransferAction->EndDelay = CardTransferFrequency;
		});
	}
}

void UCardTransferComponent::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	// CallingObject가 ACardBase 타입인 경우에만 처리
	ACardBase* CallingCard = Cast<ACardBase>(CallingObject);

	// 카드 전송 이벤트 처리 함수 호출
	RunCardTransferEvent(EventTag, CallingCard, CallTags);
}

float UCardTransferComponent::GetPriority_Implementation()
{
	return Priority;
}

void UCardTransferComponent::CancelInterruption_Implementation()
{
	bInterrupt = false;
}

void UCardTransferComponent::Interrupt_Implementation()
{
	bInterrupt = true;
}
