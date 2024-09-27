#include "UI/UW_StatBar.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "StatusSystem/StatusComponent.h"
#include "Utilities/CosGameplayTags.h"

UUW_StatBar::UUW_StatBar(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUW_StatBar::NativeConstruct()
{
	Super::NativeConstruct();
	UFunctionLibrary_Event::BindEventToGlobalDispatcherHub(this, CosGameTags::Event_Action_ModifyStatusMax);
}

void UUW_StatBar::AnimateDecrease()
{
	SmoothAnimateBarDecrease();

	// 애니메이션 바와 상태 바의 퍼센트 값이 일치하지 않으면 애니메이션을 계속 진행
	if (FMath::Abs(StatBar->GetPercent() - AnimateBar->GetPercent()) > KINDA_SMALL_NUMBER)
	{
		// 다음 틱에서 AnimateDecrease를 다시 호출
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			AnimateDecrease();
		});
	}
}

void UUW_StatBar::UpdateNumberText(int32 NewValue)
{
	// 다국어 지원을 위한 텍스트 포맷 사용
	FText StatText = FText::Format(
		NSLOCTEXT("UW_StatBar", "StatFormat", "{0} / {1}"),
		FText::AsNumber(NewValue), // 현재 값
		FText::AsNumber(MaxStatusValue) // 최대 값
	);

	// 상태 바 텍스트 설정
	NumberText->SetText(StatText);
}

float UUW_StatBar::GetBarPercent()
{
	// MaxStatusValue가 0이면, 0.0f를 반환하여 안전하게 처리
	if (MaxStatusValue == 0)
	{
		return 0.0f;
	}

	// 퍼센트 값 계산 및 클램핑
	float Percent = StatusComponent->StatusValue / static_cast<float>(MaxStatusValue);
	return FMath::Clamp(Percent, 0.0f, 1.0f);
}

void UUW_StatBar::SmoothAnimateBarDecrease()
{
	// 현재 StatBar와 AnimateBar의 퍼센트 값 가져오기
	float StatBarPercent = StatBar->GetPercent();
	float AnimateBarPercent = AnimateBar->GetPercent();

	// StatBarPercent와 AnimateBarPercent 간의 차이를 기반으로 감소 속도를 설정
	float DeltaPercent = (StatBarPercent - AnimateBarPercent) * 0.1f;

	// AnimateBar의 새로운 퍼센트 값 계산 (StatBarPercent를 향해 점진적으로 감소)
	float TargetPercent = AnimateBarPercent + DeltaPercent;

	// AnimateBar의 퍼센트 값을 업데이트
	AnimateBar->SetPercent(TargetPercent);
}

void UUW_StatBar::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	if (EventTag == CosGameTags::Event_Action_ModifyStatusMax)
	{
		if (IsValid(StatusComponent))
		{
			IInterface_StatusIcon::Execute_InitStatusWidget(this, StatusComponent, FGameplayTag(), false);
		}
		return;
	}
}

void UUW_StatBar::InitStatusWidget_Implementation(UStatusComponent* InStatus, FGameplayTag InStatusBarTag, bool bIsShowImmediately)
{
	// 상태 컴포넌트 설정
	StatusComponent = InStatus;

	// MaxStatusValue 설정: MaxAble 여부에 따라 다르게 설정
	MaxStatusValue = StatusComponent->bMaxAble ? StatusComponent->MaxValue : StatusComponent->StatusValue;

	// StatBar 색상 설정
	StatBar->SetFillColorAndOpacity(StatusComponent->Tint);

	// StatBar와 AnimateBar의 초기 퍼센트 설정
	float NewPercent = FMath::Clamp(StatusComponent->StatusValue / static_cast<float>(MaxStatusValue), 0.0f, 1.0f);
	StatBar->SetPercent(NewPercent);
	AnimateBar->SetPercent(NewPercent);

	// 텍스트 업데이트
	UpdateNumberText(StatusComponent->StatusValue);
}

bool UUW_StatBar::ModifyStatusWidget_Implementation(int32 NewValue, const FText& TextOverride, bool bRefreshAppearance, const FStatusAppearance& StatusAppearanceIn, bool bCanBeZero)
{
	// 상태 값 텍스트 업데이트
	UpdateNumberText(NewValue);

	// 상태 값에 따른 퍼센트 계산 (0 이상 1 이하로 클램프 처리)
	float ClampedPercent = FMath::Clamp(NewValue / static_cast<float>(MaxStatusValue), bCanBeZero ? 0.0f : KINDA_SMALL_NUMBER, 1.0f);

	// StatBar의 퍼센트 값 업데이트
	StatBar->SetPercent(ClampedPercent);

	// 애니메이션 시작
	AnimateDecrease();

	return true;
}
