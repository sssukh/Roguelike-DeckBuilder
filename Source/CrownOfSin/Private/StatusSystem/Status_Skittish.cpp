#include "StatusSystem/Status_Skittish.h"

#include "Core/DispatcherHubComponent.h"
#include "Interfaces/Interface_CardTarget.h"
#include "Libraries/AssetPath.h"
#include "StatusSystem/Status_Fear.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


// Sets default values for this component's properties
UStatus_Skittish::UStatus_Skittish()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	static ConstructorHelpers::FObjectFinder<UTexture2D> T_Skittish(*AssetPath::Texture::T_Skittish);
	if (T_Skittish.Succeeded())
	{
		Icon = T_Skittish.Object;
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("T_Skittish를 로드하지 못했습니다."))
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Tooltips_Statuses(*AssetPath::DataTable::DT_Tooltips_Statuses);
	if (DT_Tooltips_Statuses.Succeeded())
	{
		FToolTipValue NewToolTipValue;
		NewToolTipValue.ToolTipTable.DataTable = DT_Tooltips_Statuses.Object;
		NewToolTipValue.ToolTipTable.RowName = FName(TEXT("Skittish"));
		NewToolTipValue.bValued = true;
		Tooltips.Add(NewToolTipValue);

		FToolTipValue NewToolTipFear;
		NewToolTipFear.ToolTipTable.DataTable = DT_Tooltips_Statuses.Object;
		NewToolTipFear.ToolTipTable.RowName = FName(TEXT("Fear"));
		NewToolTipFear.bValued = false;
		Tooltips.Add(NewToolTipFear);
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("DT_Tooltips_Statuses를 로드하지 못했습니다"));
	}

	Tint = FLinearColor(1, 1, 1, 1);
	bCanBeZero = true;
	GameplayTags = FGameplayTagContainer(CosGameTags::Effect_Debuff);
}

void UStatus_Skittish::BeginPlay()
{
	Super::BeginPlay();

	UDispatcherHubComponent* DispatcherHub;
	if (GetOwnersDispatcherHub(DispatcherHub))
	{
		FGameplayTagContainer EventTags;
		EventTags.AddTag(CosGameTags::Event_PostTakeDamage);
		EventTags.AddTag(CosGameTags::Event_TurnStart);
		DispatcherHub->BindMultipleEventsToHub(this, EventTags);
	}
}

void UStatus_Skittish::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	// CallingObject가 유효한지 확인
	if (!IsValid(CallingObject)) return;

	// 턴 시작 이벤트 처리: 매 턴마다 한 번만 동작하도록 플래그 리셋
	if (EventTag == CosGameTags::Event_TurnStart)
	{
		bDoOnce = true;
		return; // 턴 시작 시 추가 작업 없음
	}

	// 피해를 받은 후 처리
	if (EventTag == CosGameTags::Event_PostTakeDamage)
	{
		// 한 번만 실행되도록 체크
		if (bDoOnce)
		{
			bDoOnce = false;

			// CallingObject를 UActorComponent로 캐스팅
			UActorComponent* DamagedComponent = Cast<UActorComponent>(CallingObject);
			COS_IF_CHECK_VOID(DamagedComponent, TEXT("SourceObject를 UActorComponent로 캐스팅할 수 없습니다."));

			// 피해를 받은 소유자와 일치하는지 확인
			if (DamagedComponent->GetOwner() == GetOwner())
			{
				// Fear 상태를 소유자에게 부여
				IInterface_CardTarget::Execute_AddToStatus(GetOwner(), UStatus_Fear::StaticClass(), StatusValue, true, nullptr);
			}
		}
	}
}
