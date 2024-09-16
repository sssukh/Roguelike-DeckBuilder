#include "StatusSystem/Status_Draw.h"

#include "CardSystem/Piles/PileHandComponent.h"
#include "Core/DispatcherHubComponent.h"
#include "Utilities/CosGameplayTags.h"


UStatus_Draw::UStatus_Draw(): PileHandReference(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UStatus_Draw::BeginPlay()
{
	Super::BeginPlay();

	// ...
	PileHandReference = GetOwner()->FindComponentByClass<UPileHandComponent>();
	if (PileHandReference)
	{
		FGameplayTagContainer EventTags;
		EventTags.AddTag(CosGameTags::Event_TurnStart);
		EventTags.AddTag(CosGameTags::Event_TurnEnd);

		UDispatcherHubComponent* DispatcherHub;
		if (GetOwnersDispatcherHub(DispatcherHub))
		{
			DispatcherHub->BindMultipleEventsToHub(this, EventTags);
		}
	}
}

void UStatus_Draw::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	// 턴이 시작되면 StatusValue만큼 카드를 뽑습니다.
	if (EventTag == CosGameTags::Event_TurnStart)
	{
		// StatusValue는 뽑을 카드 수를 나타냄
		for (int32 DrawIndex = 0; DrawIndex < StatusValue; ++DrawIndex)
		{
			// 핸드에 카드를 추가 시도
			PileHandReference->AttemptDraw();
		}
	}
	// 턴이 종료되면 현재 손에 있는 모든 카드를 버립니다.
	else if (EventTag == CosGameTags::Event_TurnEnd)
	{
		// 핸드를 비웁니다.
		PileHandReference->DiscardHand();
	}
}
