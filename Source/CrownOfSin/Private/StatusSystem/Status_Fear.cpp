#include "StatusSystem/Status_Fear.h"

#include "CardSystem/CardBase.h"
#include "CardSystem/CardPlayer.h"
#include "Core/DispatcherHubComponent.h"
#include "Libraries/AssetPath.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


// Sets default values for this component's properties
UStatus_Fear::UStatus_Fear()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	static ConstructorHelpers::FObjectFinder<UTexture2D> T_Fear(*AssetPath::Texture::T_Fear);
	if (T_Fear.Succeeded())
	{
		Icon = T_Fear.Object;
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("T_Fear를 로드하지 못했습니다."))
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Tooltips_Statuses(*AssetPath::DataTable::DT_Tooltips_Statuses);
	if (DT_Tooltips_Statuses.Succeeded())
	{
		FToolTipValue NewToolTipValue;
		NewToolTipValue.ToolTipTable.DataTable = DT_Tooltips_Statuses.Object;
		NewToolTipValue.ToolTipTable.RowName = FName(TEXT("Fear"));
		NewToolTipValue.bValued = true;
		Tooltips.Add(NewToolTipValue);
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("DT_Tooltips_Statuses를 로드하지 못했습니다"));
	}

	Tint = FLinearColor::White;
	TextAlignment = EStatusTextAlignment::BottomRight;
	Priority = 41.0f;
	FriendlyName = FText::FromString(TEXT("Fear"));
	GameplayTags = FGameplayTagContainer(CosGameTags::Effect_Debuff);
}


void UStatus_Fear::BeginPlay()
{
	FGameplayTagContainer EventTags;
	EventTags.AddTag(CosGameTags::Event_Card_Draw);
	EventTags.AddTag(CosGameTags::Event_Card_GenerateCard);
	EventTags.AddTag(CosGameTags::Event_Card_ModifyInHand);
	UFunctionLibrary_Event::BindMultipleEventsToGlobalDispatcherHub(this, EventTags);

	UDispatcherHubComponent* DispatcherHub;
	if (GetOwnersDispatcherHub(DispatcherHub))
	{
		DispatcherHub->BindEventToHub(this, CosGameTags::Event_TurnStart);
	}

	Super::BeginPlay();
}

int32 UStatus_Fear::AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, bool bRefreshAppearance, UObject* InPayload)
{
	int32 AddStatusValue = Super::AddStatusValue(InAmount, bShowSplashNumber, bShowSplashIcon, bRefreshAppearance, InPayload);

	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	UPileHandComponent* PileHandComponent = CardPlayer->FindComponentByClass<UPileHandComponent>();
	for (ACardBase* Card : PileHandComponent->Cards)
	{
		Card->CallLocalEventOnCard(CosGameTags::Event_Card_ModifyInHand, ECallGlobal::CallBefore);
	}
	return AddStatusValue;
}

void UStatus_Fear::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	if (EventTag == CosGameTags::Event_Card_Draw || EventTag == CosGameTags::Event_Card_GenerateCard || EventTag == CosGameTags::Event_Card_ModifyInHand)
	{
		ACardBase* CallingCard = Cast<ACardBase>(CallingObject);
		COS_IF_CHECK_VOID(CallingCard, TEXT("CallingObject가 ACardBase로 형변환에 실패했습니다"));
		if (CallingCard->GetOwner() == GetOwner())
		{
			if (CallingCard->CardType == CosGameTags::Effect_Attack)
			{
				int32 EffectValue;
				if (CallingCard->GetCardEffectValue(FGameplayTagContainer(), FGameplayTagContainer(CosGameTags::Effect_Attack), ECardDataType::Hand, EffectValue))
				{
					CallingCard->ModifyCardEffectValues(((EffectValue * 0.5) * -1.0f), ECardDataType::Hand, FGameplayTagContainer(), FGameplayTagContainer(CosGameTags::Effect_Attack));
				}
			}
		}
	}
	else if (EventTag == CosGameTags::Event_TurnStart)
	{
		if (CallingObject == GetOwner())
		{
			SubtractStatusValue(1);
		}
	}
}
