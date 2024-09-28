#include "StatusSystem/Status_Venomous.h"

#include "Core/DispatcherHubComponent.h"
#include "Interfaces/Interface_CardTarget.h"
#include "Libraries/AssetPath.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "StatusSystem/Status_Health.h"
#include "StatusSystem/Status_Poisoned.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


// Sets default values for this component's properties
UStatus_Venomous::UStatus_Venomous()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	static ConstructorHelpers::FObjectFinder<UTexture2D> T_Venomous(*AssetPath::Texture::T_Venomous);
	if (T_Venomous.Succeeded())
	{
		Icon = T_Venomous.Object;
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("T_Venomous를 로드하지 못했습니다."))
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Tooltips_Statuses(*AssetPath::DataTable::DT_Tooltips_Statuses);
	if (DT_Tooltips_Statuses.Succeeded())
	{
		FToolTipValue NewToolTipValue;
		NewToolTipValue.ToolTipTable.DataTable = DT_Tooltips_Statuses.Object;
		NewToolTipValue.ToolTipTable.RowName = FName(TEXT("Venomous"));
		NewToolTipValue.bValued = true;
		Tooltips.Add(NewToolTipValue);

		FToolTipValue NewToolTipPoison;
		NewToolTipPoison.ToolTipTable.DataTable = DT_Tooltips_Statuses.Object;
		NewToolTipPoison.ToolTipTable.RowName = FName(TEXT("Poison"));
		NewToolTipPoison.bValued = false;
		Tooltips.Add(NewToolTipPoison);
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("DT_Tooltips_Statuses를 로드하지 못했습니다"));
	}
	TextAlignment = EStatusTextAlignment::BottomRight;
	Priority = 500.0f;
	FriendlyName = FText::FromString(TEXT("Venomous"));
	GameplayTags = FGameplayTagContainer(CosGameTags::Effect_Buff);
}


void UStatus_Venomous::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UDispatcherHubComponent* DispatcherHub;
	if (GetOwnersDispatcherHub(DispatcherHub))
	{
		FGameplayTagContainer EventTags;
		EventTags.AddTag(CosGameTags::Event_PostAttack);
		EventTags.AddTag(CosGameTags::Event_PreAttack);
		DispatcherHub->BindMultipleEventsToHub(this, EventTags);
	}

	UFunctionLibrary_Event::BindEventToGlobalDispatcherHub(this, CosGameTags::Event_PostTakeDamage);
}

void UStatus_Venomous::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	// 피해를 받은 후의 이벤트를 처리
	if (EventTag == CosGameTags::Event_PostTakeDamage)
	{
		// RunEventGate가 열려 있을 때만 실행
		if (RunEventGate.IsOpen())
		{
			// SourceObject를 UStatus_Health로 캐스팅하여 유효성 확인
			UStatus_Health* HealthStatusComponent = Cast<UStatus_Health>(CallingObject);
			COS_IF_CHECK_VOID(HealthStatusComponent, TEXT("SourceObject를 UStatus_Health로 캐스팅할 수 없습니다."));

			// 피해가 음수(즉, 체력이 감소한 상태)이며 소유자가 아닌 경우 처리
			if (HealthStatusComponent->IncomingStatusChange < 0 && HealthStatusComponent->GetOwner() != GetOwner())
			{
				// SplashDelay 이후 독 상태를 부여
				UFunctionLibrary_Singletons::QueueDelay(this, SplashDelay);

				// 대상에게 독 상태를 추가
				IInterface_CardTarget::Execute_AddToStatus(
					HealthStatusComponent->GetOwner(),
					UStatus_Poisoned::StaticClass(),
					StatusValue, true, nullptr
				);
			}
		}
	}
	// 공격 전 이벤트 처리: 이벤트 게이트 열기
	else if (EventTag == CosGameTags::Event_PreAttack)
	{
		RunEventGate.Open();
	}
	// 공격 후 이벤트 처리: 이벤트 게이트 닫기
	else if (EventTag == CosGameTags::Event_PostAttack)
	{
		RunEventGate.Close();
	}
	
}
