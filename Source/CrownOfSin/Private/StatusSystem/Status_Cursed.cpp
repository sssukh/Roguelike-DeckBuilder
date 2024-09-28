#include "StatusSystem/Status_Cursed.h"

#include "Core/DispatcherHubComponent.h"
#include "Libraries/AssetPath.h"
#include "StatusSystem/Status_Health.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


// Sets default values for this component's properties
UStatus_Cursed::UStatus_Cursed()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	static ConstructorHelpers::FObjectFinder<UTexture2D> T_Cursed(*AssetPath::Texture::T_Cursed);
	if (T_Cursed.Succeeded())
	{
		Icon = T_Cursed.Object;
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
		NewToolTipValue.ToolTipTable.RowName = FName(TEXT("Curse"));
		NewToolTipValue.bValued = true;
		Tooltips.Add(NewToolTipValue);
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("DT_Tooltips_Statuses를 로드하지 못했습니다"));
	}

	Tint = FLinearColor::Red;
	TextAlignment = EStatusTextAlignment::BottomRight;
	FriendlyName = FText::FromString(TEXT("Curse"));
	GameplayTags = FGameplayTagContainer(CosGameTags::Effect_Debuff);
}


void UStatus_Cursed::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UDispatcherHubComponent* DispatcherHub;
	if (GetOwnersDispatcherHub(DispatcherHub))
	{
		DispatcherHub->BindEventToHub(this, CosGameTags::Event_TurnStart);
		DispatcherHub->BindEventToHub(this, CosGameTags::Event_PreTakeDamage);
	}
}

void UStatus_Cursed::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	if (EventTag == CosGameTags::Event_TurnStart)
	{
		if (CallingObject == GetOwner())
		{
			SubtractStatusValue(1);
		}
	}
	else if (EventTag == CosGameTags::Event_PreTakeDamage)
	{
		UStatus_Health* StatusHealth = Cast<UStatus_Health>(CallingObject);
		COS_IF_CHECK_VOID(StatusHealth, TEXT("CallingObject를 StatusHealth에 형변환에 실패했습니다"));
		StatusHealth->IncomingStatusChange = StatusHealth->IncomingStatusChange * 2;
	}
}
