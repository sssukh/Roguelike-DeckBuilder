#include "StatusSystem/StatusComponent.h"

#include "CardSystem/CardActions/Action_ModifyStatus.h"
#include "Core/DispatcherHubComponent.h"
#include "Interfaces/Interface_CardTarget.h"
#include "Interfaces/Widget/Interface_StatusWidget.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Libraries/AssetTableRef.h"
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

	if (UTexture2D* T_CrossMark = FAssetReferenceUtility::LoadAssetFromDataTable<UTexture2D>(AssetRefPath::TexturesPath, FName("T_CrossMark")))
	{
		Icon = T_CrossMark;
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

void UStatusComponent::UpdateLoopGuard()
{
	CurrentTriggersThisTick = CurrentTriggersThisTick + 1;


	if (!RetriggerDelay_LoopGuard)
		RetriggerDelay_LoopGuard = NewObject<URetriggerAbleDelay>();

	RetriggerDelay_LoopGuard->RetriggerDelay(0.0f, [this]()
	{
		if (CurrentTriggersThisTick >= MaxTriggersPerTick)
		{
			COS_LOG_SCREEN_ERROR(TEXT("경고: %s이(가) 최대 트리거 제한인 %d에 도달했습니다."), *GetReadableName(), MaxTriggersPerTick);
		}

		CurrentTriggersThisTick = 0;
	});
}

bool UStatusComponent::CheckLoopGuard()
{
	UpdateLoopGuard();

	return CurrentTriggersThisTick <= MaxTriggersPerTick;
}

int32 UStatusComponent::AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, bool bRefreshAppearance, UObject* InPayload)
{
	UDispatcherHubComponent* DispatcherHub = GetOwnersDispatcherHub();
	if (!DispatcherHub)
	{
		COS_SCREEN(TEXT("Owner가 DispatcherHub를 가지고 있지 않습니다."));
		return 0;
	}

	DispatcherHub->CallEvent(CosGameTags::Event_PreModifyStatus, this);

	if (bInterrupt)
	{
		bInterrupt = false;
	}
	else
	{
		IncomingStatusChange = InAmount;
		int32 LocalOldStatusValue = StatusValue;
		float LocalSplashIconDelay = 0.3f;

		StatusValue = FMath::Max(InAmount + StatusValue, 0);

		FTransform SpawnTransform = FTransform::Identity;
		if (AAction_ModifyStatus* NewActionModifyStatus = GetWorld()->SpawnActorDeferred<AAction_ModifyStatus>(AAction_ModifyStatus::StaticClass(), SpawnTransform,
		                                                                                                       nullptr, nullptr,
		                                                                                                       ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
		{
			NewActionModifyStatus->Status = this;
			NewActionModifyStatus->NewValue = StatusValue;
			NewActionModifyStatus->bShowSplashIcon = bShowSplashIcon;
			NewActionModifyStatus->bShowSplashNumber = ((InAmount != 0 || bCanBeZero) && bShowSplashNumber);

			FString InAmountString = UKismetStringLibrary::Conv_IntToString(FMath::Abs(InAmount));

			FString FriendlyString = FriendlyName.ToString();
			FriendlyString.Append(FString(TEXT(" ")));
			FriendlyString.Append(InAmountString);
			FString FriendlySelectString = FriendlyName.IsEmpty() ? InAmountString : FriendlyString;
			FText FullString = UKismetTextLibrary::Conv_StringToText(FriendlySelectString);
			FText ShowSplashText = bCanBeZero ? FriendlyName : FullString;
			NewActionModifyStatus->SplashText = bShowSplashNumber ? ShowSplashText : FText::FromString(FString(TEXT(" ")));

			NewActionModifyStatus->bRefreshAppearance = bRefreshAppearance;
			NewActionModifyStatus->bCanBeZero = bCanBeZero;
			NewActionModifyStatus->EndDelay = bShowSplashIcon ? LocalSplashIconDelay : -1.0f;

			NewActionModifyStatus->FinishSpawning(SpawnTransform);

			DispatcherHub->CallEvent(CosGameTags::Event_PostModifyStatus, this);
		}
	}


	if (StatusValue > 0 || bCanBeZero)
	{
		return StatusValue;
	}

	DestroyComponent();
	return 0;
}

int32 UStatusComponent::SubtractStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, UObject* Payload)
{
	return AddStatusValue(-InAmount, bShowSplashNumber, bShowSplashIcon, false, Payload);
}

int32 UStatusComponent::SetStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, UObject* Payload)
{
	return AddStatusValue(InAmount - StatusValue, bShowSplashNumber, bShowSplashIcon, false, Payload);
}

void UStatusComponent::SetStatusMax(int32 NewMaxValue)
{
	MaxValue = NewMaxValue;
	IInterface_CardTarget::Execute_SetStatusMax(GetOwner(), GetClass(), MaxValue);

	if (MaxValue < StatusValue)
	{
		AddStatusValue(MaxValue - StatusValue, false, false, true, nullptr);
	}

	UFunctionLibrary_Event::QueueEventInGlobalDispatcherHub(CosGameTags::Event_Action_ModifyStatusMax, this, nullptr, -1.0f, nullptr, FGameplayTagContainer());
}

void UStatusComponent::RemoveStatus()
{
	bCanBeZero = false;
	SetStatusValue(0, false, false, nullptr);
}

FStatusAppearance UStatusComponent::MakeAppearanceStruct()
{
	return FStatusAppearance(StatusValue, Icon, Tint, SlotType, TextAlignment, Tooltips, FriendlyName);
}

void UStatusComponent::GetAndSetMaxValues()
{
	bool bIsValid = IInterface_CardTarget::Execute_GetStatusMax(GetOwner(), GetClass(), MaxValue);
	if (!bIsValid)
	{
		MaxValue = StatusValue;
		IInterface_CardTarget::Execute_SetStatusMax(GetOwner(), GetClass(), MaxValue);
	}
}

UDispatcherHubComponent* UStatusComponent::GetOwnersDispatcherHub()
{
	return GetOwner()->FindComponentByClass<UDispatcherHubComponent>();
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

