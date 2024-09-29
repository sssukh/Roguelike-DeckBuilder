#include "StatusSystem/CardStatuses/Status_Card_ManaReduceOnDraw.h"

#include "CardSystem/CardBase.h"
#include "CardSystem/CardPlayer.h"
#include "CardSystem/CardUseRules/UseRule_StatCost.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "StatusSystem/Status_Mana.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


UStatus_Card_ManaReduceOnDraw::UStatus_Card_ManaReduceOnDraw()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UStatus_Card_ManaReduceOnDraw::BeginPlay()
{
	Super::BeginPlay();

	// ...

	FGameplayTagContainer EventTags;
	EventTags.AddTag(CosGameTags::Event_Card_ModifyInHand);
	EventTags.AddTag(CosGameTags::Event_Card_PrePlay);
	EventTags.AddTag(CosGameTags::Event_TurnStart);
	UFunctionLibrary_Event::BindMultipleEventsToGlobalDispatcherHub(this, EventTags);

	if (ACardBase* CastingCard = Cast<ACardBase>(GetOwner()))
	{
		OwningCard = CastingCard;
		OwningCard->BindLocalCardEvents(this, FGameplayTagContainer(CosGameTags::Event_Card_ExitHand));
	}
}

void UStatus_Card_ManaReduceOnDraw::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	if (!CardPlayer)
	{
		COS_LOG_SCREEN(TEXT("CardPlayer is nullptr in UStatus_Card_ManaReduceOnDraw::RunEvent_Implementation"));
		return;
	}

	UPileHandComponent* PileHandComponent = CardPlayer->FindComponentByClass<UPileHandComponent>();
	if (!PileHandComponent)
	{
		COS_LOG_SCREEN(TEXT("PileHandComponent is nullptr in UStatus_Card_ManaReduceOnDraw::RunEvent_Implementation"));
		return;
	}

	ACardBase* CastingCard = Cast<ACardBase>(GetOwner());
	if (!CastingCard)
	{
		COS_LOG_SCREEN(TEXT("CastingCard is nullptr in UStatus_Card_ManaReduceOnDraw::RunEvent_Implementation"));
		return;
	}

	// CastingCard가 PileHandComponent에 있는지 확인
	if (PileHandComponent->Cards.Contains(CastingCard))
	{
		// 각 이벤트 태그에 따른 처리
		if (EventTag == CosGameTags::Event_Card_PrePlay)
		{
			CurrentModification += StatusValue;
			ApplyManaCostReduction();
		}
		else if (EventTag == CosGameTags::Event_Card_ModifyInHand)
		{
			ApplyManaCostReduction();
		}
		else if (EventTag == CosGameTags::Event_Card_ExitHand)
		{
			CurrentModification = 0;
		}
	}
	else
	{
		// 턴이 시작될 때 비용을 초기화
		if (EventTag == CosGameTags::Event_TurnStart)
		{
			CurrentModification = 0;
		}
	}
}

void UStatus_Card_ManaReduceOnDraw::ApplyManaCostReduction()
{
	// OwningCard가 유효한지 먼저 확인
	if (!OwningCard)
	{
		COS_LOG_SCREEN_ERROR(TEXT("OwningCard는 UStatus_Card_ManaReduceOnDraw::HandleManaReductionOnDraw에서 nullptr입니다."));
		return;
	}

	// 덱에 있는 카드의 마나 비용을 가져옴
	int32 DeckManaCost, DeckCostRuleIndex;
	if (!OwningCard->GetStatusCostValueFromUseRules(ECardDataType::Deck, UStatus_Mana::StaticClass(), DeckManaCost, DeckCostRuleIndex))
	{
		COS_LOG_SCREEN_ERROR(TEXT("UStatus_Card_ManaReduceOnDraw::HandleManaReductionOnDraw에서 덱 마나 비용을 가져오지 못했습니다."));
		return;
	}

	// 손에 있는 카드의 마나 비용을 가져옴
	int32 HandManaCost, HandCostRuleIndex;
	if (!OwningCard->GetStatusCostValueFromUseRules(ECardDataType::Hand, UStatus_Mana::StaticClass(), HandManaCost, HandCostRuleIndex))
	{
		COS_LOG_SCREEN_ERROR(TEXT("UStatus_Card_ManaReduceOnDraw::HandleManaReductionOnDraw에서 핸드 마나 비용을 가져오지 못했습니다."));
		return;
	}

	// 새로운 마나 비용을 계산
	int32 UpdatedManaCost = FMath::Max(0, FMath::Min(HandManaCost, DeckManaCost - CurrentModification));

	// 새로운 비용을 적용
	if (OwningCard->SetUseRuleCost(UUseRule_StatCost::StaticClass(), UStatus_Mana::StaticClass(), UpdatedManaCost, ECardDataType::Hand, false))
	{
		// 카드가 수정되었다는 이벤트를 브로드캐스트
		if (OwningCard->OnThisCardModified.IsBound())
		{
			OwningCard->OnThisCardModified.Broadcast();
		}
	}


	
}
