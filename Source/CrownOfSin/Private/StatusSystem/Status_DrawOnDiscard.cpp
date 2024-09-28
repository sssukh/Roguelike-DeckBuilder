#include "StatusSystem/Status_DrawOnDiscard.h"

#include "CardSystem/CardPlayer.h"
#include "CardSystem/Piles/PileHandComponent.h"
#include "Core/DispatcherHubComponent.h"
#include "Libraries/AssetPath.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


UStatus_DrawOnDiscard::UStatus_DrawOnDiscard()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	static ConstructorHelpers::FObjectFinder<UTexture2D> T_Owl(*AssetPath::Texture::T_Owl);
	if (T_Owl.Succeeded())
	{
		Icon = T_Owl.Object;
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("T_Owl를 로드하지 못했습니다."));
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Tooltips_Statuses(*AssetPath::DataTable::DT_Tooltips_Statuses);
	if (DT_Tooltips_Statuses.Succeeded())
	{
		FToolTipValue NewToolTipValue;
		NewToolTipValue.ToolTipTable.DataTable = DT_Tooltips_Statuses.Object;
		NewToolTipValue.ToolTipTable.RowName = FName(TEXT("AvianAid"));
		NewToolTipValue.bValued = false;
		Tooltips.Add(NewToolTipValue);

		FToolTipValue NewToolTipValue2;
		NewToolTipValue2.ToolTipTable.DataTable = DT_Tooltips_Statuses.Object;
		NewToolTipValue2.ToolTipTable.RowName = FName(TEXT("Discard"));
		NewToolTipValue2.bValued = false;
		Tooltips.Add(NewToolTipValue2);
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("DT_Tooltips_Statuses를 로드하지 못했습니다"));
	}

	Tint = FLinearColor(0.1, 0.4, 1, 1);
	bCanBeZero = true;
	FriendlyName = FText::FromString(TEXT("Avian Aid"));
	GameplayTags = FGameplayTagContainer(CosGameTags::Effect_Buff);
}

void UStatus_DrawOnDiscard::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UDispatcherHubComponent* DispatcherHub;
	if (GetOwnersDispatcherHub(DispatcherHub))
	{
		DispatcherHub->BindEventToHub(this, CosGameTags::Event_Card_Discard);
	}
}

void UStatus_DrawOnDiscard::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	if (EventTag == CosGameTags::Event_Card_Discard)
	{
		if (CallTags.HasTagExact(CosGameTags::Flag_Active))
		{
			ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
			int32 LastIndex = FMath::Max(StatusValue - 1, 0);
			for (int i = 0; i < LastIndex; ++i)
			{
				CardPlayer->PileHandComponent->AttemptDraw();
			}
		}
	};
}
