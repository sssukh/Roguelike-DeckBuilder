#include "StatusSystem/Status_Mana.h"

#include "Libraries/AssetTableRef.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "Utilities/CosGameplayTags.h"

UStatus_Mana::UStatus_Mana()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	TextAlignment = EStatusTextAlignment::Center;
	bCanBeZero = true;
	FriendlyName = FText::FromString(TEXT("Mana"));

	if (UDataTable* DT_Tooltips_Statuses = FAssetReferenceUtility::LoadDataTable(FName("DT_Tooltips_Statuses")))
	{
		FToolTipValue NewToolTip;
		NewToolTip.ToolTipTable.DataTable = DT_Tooltips_Statuses;
		NewToolTip.ToolTipTable.RowName = FName("Mana");
		Tooltips.Add(NewToolTip);
	}
}

int32 UStatus_Mana::AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, bool bRefreshAppearance, UObject* InPayload)
{
	int32 AfterValue = Super::AddStatusValue(InAmount, bShowSplashNumber, bShowSplashIcon, bRefreshAppearance, InPayload);

	UFunctionLibrary_Event::QueueEventInGlobalDispatcherHub(CosGameTags::Event_Action_ResourceChange, this);

	return AfterValue;
}
