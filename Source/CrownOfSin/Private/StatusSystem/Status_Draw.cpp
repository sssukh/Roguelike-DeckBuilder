#include "StatusSystem/Status_Draw.h"

#include "CardSystem/Piles/PileHandComponent.h"
#include "Core/DispatcherHubComponent.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


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

	// 핸드 참조를 찾음
	PileHandReference = GetOwner()->FindComponentByClass<UPileHandComponent>();

	// 핸드 참조가 유효한지 확인
	if (PileHandReference)
	{
		UDispatcherHubComponent* DispatcherHub;
		if (GetOwnersDispatcherHub(DispatcherHub))
		{
			// 이벤트 태그 설정 (턴 시작 및 종료 이벤트)
			FGameplayTagContainer EventTags;
			EventTags.AddTag(CosGameTags::Event_TurnStart);
			EventTags.AddTag(CosGameTags::Event_TurnEnd);

			// 이벤트 바인딩
			DispatcherHub->BindMultipleEventsToHub(this, EventTags);
		}
		else
		{
			// DispatcherHub를 찾지 못했을 때 경고 로그 출력
			COS_LOG_SCREEN_ERROR(TEXT("DispatcherHub를 찾을 수 없습니다."));
		}
	}
	else
	{
		// 핸드 컴포넌트를 찾지 못했을 때 경고 로그 출력
		COS_LOG_SCREEN_ERROR(TEXT("PileHandComponent를 찾을 수 없습니다."));
	}
}

void UStatus_Draw::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	// 핸드 참조가 유효한지 확인
	COS_IF_CHECK(PileHandReference, TEXT("PileHandReference가 유효하지 않습니다."));

	// 턴 시작 시 카드를 뽑음
	if (EventTag == CosGameTags::Event_TurnStart)
	{
		if (StatusValue > 0) // StatusValue가 유효한지 확인
		{
			for (int32 DrawIndex = 0; DrawIndex < StatusValue; ++DrawIndex)
			{
				// 핸드에 카드를 추가 시도
				PileHandReference->AttemptDraw();
			}
		}
		else
		{
			// StatusValue가 0 이하일 경우 경고 로그 출력
			COS_LOG_WARNING(TEXT("StatusValue가 0 이하입니다. 카드를 뽑을 수 없습니다."));
		}
	}
	// 턴 종료 시 모든 카드를 버림
	else if (EventTag == CosGameTags::Event_TurnEnd)
	{
		// 핸드를 비움
		PileHandReference->DiscardHand();
	}
}
