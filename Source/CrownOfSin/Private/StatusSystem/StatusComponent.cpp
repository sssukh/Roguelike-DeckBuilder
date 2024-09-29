#include "StatusSystem/StatusComponent.h"

#include "ActionSystem/ActionManagerSubsystem.h"
#include "ActionSystem/Action_ModifyStatus.h"
#include "Core/DispatcherHubComponent.h"
#include "Interfaces/Interface_CardTarget.h"
#include "Interfaces/Widget/Interface_StatusWidget.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Libraries/AssetPath.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Libraries/RetriggerAbleDelay.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"

UStatusComponent::UStatusComponent(): OwnerUiRef(nullptr), StatusIndicator(nullptr), RetriggerDelay_LoopGuard(nullptr)
{
	// 게임 시작 시 초기화되고 매 프레임마다 틱되도록 이 구성 요소를 설정합니다. 이 기능을 설정할 수 있습니다
	// 필요하지 않은 경우 성능 향상을 위해 꺼집니다.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
	static ConstructorHelpers::FObjectFinder<UTexture2D> T_CrossMark(*AssetPath::Texture::T_CrossMark);
	if (T_CrossMark.Succeeded())
	{
		Icon = T_CrossMark.Object;
	}
	else
	{
		COS_LOG_ERROR(TEXT("T_CrossMark를 로드하지 못했습니다."));
	}

	Tint = FLinearColor(0, 1, 0, 1);
}

void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bMaxAble)
	{
		GetAndSetMaxValues();
	}

	if (IsValid(OwnerUiRef))
	{
		StatusIndicator = IInterface_StatusWidget::Execute_AddStatusIndicator(OwnerUiRef, this, bShowImmediately);
	}
}

int32 UStatusComponent::AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, bool bRefreshAppearance, UObject* InPayload)
{
	// 상태를 소유한 액터의 DispatcherHubComponent를 가져옴. 없으면 종료
	UDispatcherHubComponent* DispatcherHub;
	if (GetOwnersDispatcherHub(DispatcherHub))
	{
		// 상태 변경 전 이벤트 호출
		DispatcherHub->CallEvent(CosGameTags::Event_PreModifyStatus, this);
	}


	// 상태가 인터럽트된 경우 인터럽트를 해제
	if (bInterrupt)
	{
		bInterrupt = false;
	}
	else
	{
		// 상태 값 변경 및 시각적 효과 처리
		ApplyStatusChange(InAmount, bShowSplashNumber, bShowSplashIcon, bRefreshAppearance);

		// 상태 변경 후 이벤트 호출
		DispatcherHub->CallEvent(CosGameTags::Event_PostModifyStatus, this);
	}

	// 상태 값이 0보다 크거나, 0을 허용하는 경우 현재 상태 값을 반환
	if (StatusValue > 0 || bCanBeZero)
	{
		return StatusValue;
	}

	// 상태 값이 0 이하로 떨어지고 0을 허용하지 않으면 컴포넌트를 파괴
	DestroyComponent();
	return 0;
}

int32 UStatusComponent::SubtractStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, UObject* Payload)
{
	// 상태 값을 InAmount만큼 감소시킴. AddStatusValue에 음수 값을 전달하여 처리.
	// bRefreshAppearance는 false로 설정하여 상태 변화 시 외관을 새로 고치지 않음.
	return AddStatusValue(-InAmount, bShowSplashNumber, bShowSplashIcon, false, Payload);
}

int32 UStatusComponent::SetStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, UObject* Payload)
{
	// 상태 값을 InAmount로 설정하기 위해 현재 상태 값과의 차이를 계산
	int32 StatusDelta = InAmount - StatusValue;

	// 상태 값의 변화를 적용하고, 시각적 효과 처리
	return AddStatusValue(StatusDelta, bShowSplashNumber, bShowSplashIcon, false, Payload);
}

void UStatusComponent::SetStatusMax(int32 NewMaxValue)
{
	// 새로운 최대 상태 값 설정
	MaxValue = NewMaxValue;

	// 소유자에게 상태 최대 값을 설정하도록 요청
	if (GetOwner()->Implements<UInterface_CardTarget>())
	{
		IInterface_CardTarget::Execute_SetStatusMax(GetOwner(), GetClass(), MaxValue);
	}

	// 새로운 최대 값이 현재 상태 값보다 작으면 상태 값을 조정
	if (MaxValue < StatusValue)
	{
		// 상태 값을 새로운 최대 값에 맞게 감소
		AddStatusValue(MaxValue - StatusValue, false, false, true, nullptr);
	}

	// 상태 최대 값 변경 이벤트를 글로벌 DispatcherHub에 전파
	UFunctionLibrary_Event::QueueEventInGlobalDispatcherHub(
		CosGameTags::Event_Action_ModifyStatusMax,
		this,
		nullptr,
		-1.0f,
		nullptr,
		FGameplayTagContainer()
	);
}

void UStatusComponent::RemoveStatus()
{
	bCanBeZero = false;
	SetStatusValue(0, false, false, nullptr);
}

FStatusAppearance UStatusComponent::MakeAppearanceStruct()
{
	// 현재 상태 값을 기반으로 FStatusAppearance 구조체를 생성하여 반환
	return FStatusAppearance(
		StatusValue, // 현재 상태 값
		Icon, // 상태를 나타내는 아이콘
		Tint, // 아이콘에 적용할 색상
		SlotType, // 상태 슬롯 유형 (예: Passive, Active)
		TextAlignment, // 상태 텍스트 정렬 방식
		Tooltips, // 상태에 대한 툴팁 정보
		FriendlyName // 사용자에게 표시될 상태 이름
	);
}

void UStatusComponent::GetAndSetMaxValues()
{
	AActor* OwnerReference = GetOwner();
	FString OwnerName = UKismetSystemLibrary::GetDisplayName(OwnerReference);

	// UInterface_CardTarget를 상속받지 않은 경우
	COS_IF_CHECK_VOID(OwnerReference->Implements<UInterface_CardTarget>(), TEXT("%s 가 UInterface_CardTarget를 상속받지 않았습니다."), *OwnerName);

	int32 LocalMaxValue;
	if (IInterface_CardTarget::Execute_GetStatusMax(OwnerReference, GetClass(), LocalMaxValue))
	{
		MaxValue = LocalMaxValue;
	}
	else
	{
		MaxValue = StatusValue;
		IInterface_CardTarget::Execute_SetStatusMax(OwnerReference, GetClass(), MaxValue); // 설정한 최대 값을 소유자에 반영
	}
}

bool UStatusComponent::GetOwnersDispatcherHub(UDispatcherHubComponent*& OutDispatcherHubComponent)
{
	// 소유한 액터에서 UDispatcherHubComponent를 검색
	OutDispatcherHubComponent = GetOwner()->FindComponentByClass<UDispatcherHubComponent>();

	// DispatcherHubComponent가 유효한지 확인 후 결과 반환
	return OutDispatcherHubComponent != nullptr;
}

void UStatusComponent::ApplyStatusChange(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, bool bRefreshAppearance)
{
	// 상태 변경값을 저장하고, 상태 값이 음수로 내려가는 것을 방지
	IncomingStatusChange = InAmount;
	StatusValue = FMath::Max(InAmount + StatusValue, 0);

	// 시각적 효과를 처리
	SpawnModifyStatusAction(InAmount, bShowSplashNumber, bShowSplashIcon, bRefreshAppearance);
}

void UStatusComponent::SpawnModifyStatusAction(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, bool bRefreshAppearance)
{
	UActionManagerSubsystem* ActionManagerSubsystem = GetWorld()->GetSubsystem<UActionManagerSubsystem>();
	ActionManagerSubsystem->CreateAndQueueAction<AAction_ModifyStatus>([&](AAction_ModifyStatus* Action_ModifyStatus)
	{
		// 시각적 효과 설정
		Action_ModifyStatus->StatusReference = this;
		Action_ModifyStatus->NewValue = StatusValue;
		Action_ModifyStatus->bShowSplashIcon = bShowSplashIcon;
		Action_ModifyStatus->bShowSplashNumber = (InAmount != 0 || bCanBeZero) && bShowSplashNumber;

		// Splash 텍스트 생성
		Action_ModifyStatus->SplashText = GenerateSplashText(InAmount, bShowSplashNumber);

		// 추가 설정
		Action_ModifyStatus->bRefreshAppearance = bRefreshAppearance;
		Action_ModifyStatus->bCanBeZero = bCanBeZero;
		Action_ModifyStatus->EndDelay = bShowSplashIcon ? 0.3f : -1.0f;
	});
}

FText UStatusComponent::GenerateSplashText(int32 InAmount, bool bShowSplashNumber)
{
	// 상태 변경량을 문자열로 변환
	FString InAmountString = UKismetStringLibrary::Conv_IntToString(FMath::Abs(InAmount));

	// FriendlyName과 상태 변경량을 결합할 문자열 준비
	FString FriendlyString = FriendlyName.ToString();
	FriendlyString.Append(TEXT(" "));
	FriendlyString.Append(InAmountString);

	// FriendlyName이 없을 경우 InAmountString만 사용, 있을 경우 FriendlyString 사용
	FString FriendlySelectString = FriendlyName.IsEmpty() ? InAmountString : FriendlyString;

	// FriendlySelectString을 FText로 변환
	FText FullString = UKismetTextLibrary::Conv_StringToText(FriendlySelectString);

	// bCanBeZero가 true일 경우 FriendlyName만 반환, 아닐 경우 FullString 반환
	FText ShowSplashText = bCanBeZero ? FriendlyName : FullString;

	// bShowSplashNumber가 true일 경우 ShowSplashText 반환, 아닐 경우 빈 텍스트 반환
	return bShowSplashNumber ? ShowSplashText : FText::FromString(TEXT(" "));
}

bool UStatusComponent::CheckLoopGuard()
{
	// 루프 가드를 업데이트하여 트리거 수를 체크
	UpdateLoopGuard();

	// 현재 틱의 트리거 수가 최대 허용 트리거 수 이하인지 여부를 반환
	return CurrentTriggersThisTick <= MaxTriggersPerTick;
}

void UStatusComponent::UpdateLoopGuard()
{
	// 이번 틱에서의 트리거 횟수 증가
	CurrentTriggersThisTick++;

	// RetriggerAbleDelay 객체가 없으면 생성
	if (!RetriggerDelay_LoopGuard)
	{
		RetriggerDelay_LoopGuard = NewObject<URetriggerAbleDelay>();
	}

	// 루프 가드 재설정: 트리거 제한을 넘었는지 체크하고 틱마다 트리거 수 초기화
	RetriggerDelay_LoopGuard->RetriggerDelay(0.0f, [this]()
	{
		// 트리거 수가 최대 허용치 이상일 경우 경고 메시지 출력
		if (CurrentTriggersThisTick >= MaxTriggersPerTick)
		{
			COS_LOG_SCREEN_ERROR(TEXT("경고: %s이(가) 최대 트리거 제한인 %d에 도달했습니다."), *GetReadableName(), MaxTriggersPerTick);
		}
		// 트리거 수를 초기화
		CurrentTriggersThisTick = 0;
	});
}

bool UStatusComponent::Interact_Implementation(const FGameplayTagContainer& Tags)
{
	return false;
}

void UStatusComponent::Interrupt_Implementation()
{
	bInterrupt = true;
}

void UStatusComponent::CancelInterruption_Implementation()
{
	bInterrupt = false;
}

float UStatusComponent::GetPriority_Implementation()
{
	return Priority;
}

void UStatusComponent::SetPriority_Implementation(float InPriority)
{
	Priority = InPriority;
}

FText UStatusComponent::GetFriendlyName_Implementation()
{
	return FriendlyName;
}
