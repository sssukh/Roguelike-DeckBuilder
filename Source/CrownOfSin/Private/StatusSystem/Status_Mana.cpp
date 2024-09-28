#include "StatusSystem/Status_Mana.h"

#include "Libraries/AssetPath.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"

UStatus_Mana::UStatus_Mana()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	TextAlignment = EStatusTextAlignment::Center;
	bCanBeZero = true;
	FriendlyName = FText::FromString(TEXT("Mana"));

	// 툴팁 데이터 테이블 로딩
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Tooltips_Statuses(*AssetPath::DataTable::DT_Tooltips_Statuses);
	if (DT_Tooltips_Statuses.Succeeded())
	{
		FToolTipValue NewToolTip;
		NewToolTip.ToolTipTable.DataTable = DT_Tooltips_Statuses.Object;
		NewToolTip.ToolTipTable.RowName = FName("Deck");
		NewToolTip.bValued = false;
		Tooltips.Add(NewToolTip);
	}
	else
	{
		COS_LOG_ERROR(TEXT("DT_Tooltips_Statuses 데이터 테이블을 로드하지 못했습니다."));
	}
}

int32 UStatus_Mana::AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, bool bRefreshAppearance, UObject* InPayload)
{
	int32 AfterValue = Super::AddStatusValue(InAmount, bShowSplashNumber, bShowSplashIcon, bRefreshAppearance, InPayload);

	UFunctionLibrary_Event::QueueEventInGlobalDispatcherHub(CosGameTags::Event_Action_ResourceChange, this);

	return AfterValue;
}
