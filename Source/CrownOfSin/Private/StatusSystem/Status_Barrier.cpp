#include "StatusSystem/Status_Barrier.h"

#include "Core/DispatcherHubComponent.h"
#include "Libraries/AssetPath.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"

UStatus_Barrier::UStatus_Barrier()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	static ConstructorHelpers::FObjectFinder<UTexture2D> T_Barrier(*AssetPath::Texture::T_Barrier);
	if (T_Barrier.Succeeded())
	{
		Icon = T_Barrier.Object;
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("T_Barrier를 로드하지 못했습니다."))
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Tooltips_Statuses(*AssetPath::DataTable::DT_Tooltips_Statuses);
	if (DT_Tooltips_Statuses.Succeeded())
	{
		FToolTipValue NewToolTipValue;
		NewToolTipValue.ToolTipTable.DataTable = DT_Tooltips_Statuses.Object;
		NewToolTipValue.ToolTipTable.RowName = FName(TEXT("Barrier"));
		NewToolTipValue.bValued = true;
		Tooltips.Add(NewToolTipValue);
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("DT_Tooltips_Statuses를 로드하지 못했습니다"));
	}

	Tint = FLinearColor(1, 1, 1, 1);
	TextAlignment = EStatusTextAlignment::BottomRight;
	Priority = 500.0f;
	FriendlyName = FText::FromString(TEXT("Barrier"));
	GameplayTags = FGameplayTagContainer(CosGameTags::Effect_Buff);
}

void UStatus_Barrier::BeginPlay()
{
	Super::BeginPlay();

	// ...

	UDispatcherHubComponent* DispatcherHub;
	if (GetOwnersDispatcherHub(DispatcherHub))
	{
		DispatcherHub->BindEventToHub(this, CosGameTags::Event_PreTakeDamage);
	}
}

void UStatus_Barrier::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	if (EventTag == CosGameTags::Event_PreTakeDamage)
	{
		UStatusComponent* CastStatusComponent = Cast<UStatusComponent>(CallingObject);
		COS_IF_CHECK_VOID(CastStatusComponent, TEXT("CastStatusComponent로 형변환에 실패했습니다."));

		if (CastStatusComponent->IncomingStatusChange < 0)
		{
			CastStatusComponent->IncomingStatusChange = 0;

			SubtractStatusValue(1, false, false, nullptr);
		}
	}
}
