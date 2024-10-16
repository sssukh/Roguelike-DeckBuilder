﻿#include "StatusSystem/Status_AttackPattern.h"

#include "ActionSystem/ActionManagerSubsystem.h"
#include "ActionSystem/Action_ModifyStatus.h"
#include "CardSystem/CardBase.h"
#include "CombatSystem/AttackPatternComponent.h"
#include "Core/DispatcherHubComponent.h"
#include "Libraries/AssetPath.h"

#include "Libraries/FunctionLibrary_Singletons.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


UStatus_AttackPattern::UStatus_AttackPattern(): LastCardRepetitions(0), LastDisplayedStatusValue(0), NextCard(nullptr), AttackPatternReference(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	SlotType = EStatusSlot::Active;
	Icon = nullptr;
	Tint = FLinearColor(0, 0, 0, 0);
	Priority = 40.0f;
	bCanBeZero = true;

	FriendlyName = FText::FromString(TEXT("Attack Nearest"));
	
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Tooltips_Statuses(*AssetPath::DataTable::DT_Tooltips_Statuses);
	if (DT_Tooltips_Statuses.Succeeded())
	{
		FToolTipValue NewToolTip;
		NewToolTip.ToolTipTable.DataTable = DT_Tooltips_Statuses.Object;
		NewToolTip.ToolTipTable.RowName = FName(TEXT("Unknown"));
		NewToolTip.bValued = true;
		Tooltips.Add(NewToolTip);
	}
}

void UStatus_AttackPattern::BeginPlay()
{
	AttackPatternReference = GetOwner()->FindComponentByClass<UAttackPatternComponent>();
	if (!AttackPatternReference) return;

	Super::BeginPlay();

	UDispatcherHubComponent* DispatcherHub;
	if (GetOwnersDispatcherHub(DispatcherHub))
	{
		FGameplayTagContainer EnventTags;
		EnventTags.AddTag(CosGameTags::Event_PostModifyStatus);
		EnventTags.AddTag(CosGameTags::Event_TurnStart);
		DispatcherHub->BindMultipleEventsToHub(this, EnventTags);

		NextCard = AttackPatternReference->GetNextCard();
	}
}

void UStatus_AttackPattern::HandleRunEvent()
{
	if (!IsValid(NextCard))
	{
		return; // 카드가 유효하지 않으면 처리 종료
	}

	// 카드에 로컬 이벤트 발생
	NextCard->CallLocalEventOnCard(CosGameTags::Event_Card_GenerateCard, ECallGlobal::CallAfter);

	// 외형이 변경되었는지 확인
	bAppearanceChanged = HasAppearanceChanged();

	// 카드의 속성 업데이트
	Icon = NextCard->GetCardPortrait(ECardDataType::Base);
	Tint = NextCard->GetCardFrameTint(ECardDataType::Base).GetSpecifiedColor();
	LastCardRepetitions = NextCard->GetCardRepetitions(ECardDataType::Hand);

	// 툴팁 정보 업데이트
	UpdateTooltipsFromCard(NextCard);

	// 카드가 공격 타입일 경우 상태 값과 텍스트 정렬을 업데이트
	if (NextCard->CardType == CosGameTags::Effect_Attack)
	{
		NextCard->GetCardEffectValue(FGameplayTagContainer(), FGameplayTagContainer(CosGameTags::Effect_Attack), ECardDataType::Hand, StatusValue);
		TextAlignment = EStatusTextAlignment::BottomLeft;
	}
	else
	{
		TextAlignment = EStatusTextAlignment::None;
	}

	// 상태값이 변했거나 외형이 바뀌었을 경우 상태 수정 액션 큐에 추가
	if (CurrentEventTag == CosGameTags::Event_TurnStart || StatusValue != LastDisplayedStatusValue || bAppearanceChanged)
	{
		// 지연 큐에 추가
		UFunctionLibrary_Singletons::QueueDelay(this, PreDisplayDelay);

		// 상태 수정 액션을 생성하고 큐에 추가
		UActionManagerSubsystem* ActionManagerSubsystem = GetWorld()->GetSubsystem<UActionManagerSubsystem>();
		ActionManagerSubsystem->CreateAndQueueAction<AAction_ModifyStatus>([&](AAction_ModifyStatus* ModifyStatusAction)
		{
			ModifyStatusAction->NewValue = StatusValue;
			ModifyStatusAction->StatusReference = this;
			ModifyStatusAction->bShowSplashIcon = false;
			ModifyStatusAction->bShowSplashNumber = false;
			ModifyStatusAction->bRefreshAppearance = true;
			ModifyStatusAction->bCanBeZero = true;
			ModifyStatusAction->EndDelay = -1.0f;

			// 상태값과 카드 반복 횟수를 텍스트로 변환
			FString StatusText = FString::Printf(TEXT("%d"), StatusValue);
			FString RepetitionText = FString::Printf(TEXT("%d"), LastCardRepetitions + 1);

			// "상태값 x 반복횟수" 형식의 텍스트 생성
			FString CombinedText = FString::Printf(TEXT("%s x %s"), *StatusText, *RepetitionText);
			FText FinalTooltipText = FText::AsCultureInvariant(CombinedText);

			// 반복 횟수가 0보다 큰 경우 툴팁 텍스트 표시, 그렇지 않으면 빈 문자열
			ModifyStatusAction->TextOverride = (LastCardRepetitions > 0 && NextCard->CardType == CosGameTags::Effect_Attack)
				                                   ? FinalTooltipText
				                                   : FText::FromString(TEXT(""));
		});

		// 마지막으로 표시된 상태 값을 업데이트
		LastDisplayedStatusValue = StatusValue;
	}
}

bool UStatus_AttackPattern::HasAppearanceChanged()
{
	// 아이콘이 변경되었는지 확인
	bool bIconChanged = Icon != NextCard->GetCardPortrait(ECardDataType::Base);

	// 색상 틴트가 변경되었는지 확인
	bool bTintChanged = Tint != NextCard->GetCardFrameTint(ECardDataType::Base).GetSpecifiedColor();

	// 카드 반복 횟수가 변경되었는지 확인
	bool bRepetitionsChanged = LastCardRepetitions != NextCard->GetCardRepetitions(ECardDataType::Hand);

	// 하나라도 변경된 항목이 있으면 true를 반환
	return bIconChanged || bTintChanged || bRepetitionsChanged;
}

void UStatus_AttackPattern::UpdateTooltipsFromCard(ACardBase* InCard)
{
	// 새로운 툴팁 데이터를 저장할 임시 배열
	TArray<FToolTipValue> UpdatedTooltips;

	// 카드에서 툴팁 데이터를 가져옴
	const TArray<FDataTableRowHandle>& CardTooltips = InCard->GetCardTooltips(ECardDataType::Hand);

	// 툴팁 데이터를 변환하여 로컬 배열에 추가
	for (const FDataTableRowHandle& TooltipRow : CardTooltips)
	{
		UpdatedTooltips.Add(FToolTipValue(TooltipRow, false));
	}

	// 멤버 변수 Tooltips에 로컬 배열을 적용
	Tooltips = UpdatedTooltips;
}

void UStatus_AttackPattern::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	// 현재 이벤트 태그를 저장
	CurrentEventTag = EventTag;

	// 턴 시작 이벤트 처리
	if (CurrentEventTag == CosGameTags::Event_TurnStart)
	{
		// 이벤트가 소유자의 로컬 이벤트인지 확인
		if (CallingObject == GetOwner() && !bIsGlobal)
		{
			// 카드가 유효한지 검사
			if (!IsValid(NextCard))
			{
				COS_LOG_SCREEN_ERROR(TEXT("경고: 공격 패턴 구성 요소에 유효한 카드가 없습니다."));
				return;
			}

			// 현재 카드를 사용 시도
			NextCard->AttemptUseCard(EmptyArray, false, false, false);

			// 공격 패턴 컴포넌트가 유효한지 확인
			if (!IsValid(AttackPatternReference)) return;

			// 다음 카드를 설정
			NextCard = AttackPatternReference->GetNextCard();

			// 이벤트 처리
			HandleRunEvent();
		}
	}
	// 상태 수정 후 이벤트 처리
	else if (CurrentEventTag == CosGameTags::Event_PostModifyStatus)
	{
		// 카드가 유효한지 검사
		if (!IsValid(NextCard)) return;

		// 카드의 핸드 내 수정사항 초기화
		NextCard->ResetInHandModifications();

		// 이벤트 처리
		HandleRunEvent();
	}
}
