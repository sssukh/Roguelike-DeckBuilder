// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusSystem/Status_Strength.h"

#include "CardSystem/CardBase.h"
#include "CardSystem/CardPlayer.h"
#include "Libraries/AssetPath.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


// Sets default values for this component's properties
UStatus_Strength::UStatus_Strength()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	static ConstructorHelpers::FObjectFinder<UTexture2D> T_Strength(*AssetPath::Texture::T_Strength);
	if (T_Strength.Succeeded())
	{
		Icon = T_Strength.Object;
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("T_Strength를 로드하지 못했습니다."))
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Tooltips_Statuses(*AssetPath::DataTable::DT_Tooltips_Statuses);
	if (DT_Tooltips_Statuses.Succeeded())
	{
		FToolTipValue NewToolTipValue;
		NewToolTipValue.ToolTipTable.DataTable = DT_Tooltips_Statuses.Object;
		NewToolTipValue.ToolTipTable.RowName = FName(TEXT("Strength"));
		NewToolTipValue.bValued = true;
		Tooltips.Add(NewToolTipValue);
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("DT_Tooltips_Statuses를 로드하지 못했습니다"));
	}

	Tint = FLinearColor(1, 1, 1, 1);
	TextAlignment = EStatusTextAlignment::BottomRight;
	Priority = 50.0f;
	FriendlyName = FText::FromString(TEXT("Strength"));
	GameplayTags = FGameplayTagContainer(CosGameTags::Effect_Buff);
}


void UStatus_Strength::BeginPlay()
{
	// ...
	FGameplayTagContainer EventTags;
	EventTags.AddTag(CosGameTags::Event_Card_Draw);
	EventTags.AddTag(CosGameTags::Event_Card_GenerateCard);
	EventTags.AddTag(CosGameTags::Event_Card_ModifyInHand);

	UFunctionLibrary_Event::BindMultipleEventsToGlobalDispatcherHub(this, EventTags);

	Super::BeginPlay();
}

int32 UStatus_Strength::AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, bool bRefreshAppearance, UObject* InPayload)
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

void UStatus_Strength::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	if (EventTag == CosGameTags::Event_Card_Draw || EventTag == CosGameTags::Event_Card_GenerateCard || EventTag == CosGameTags::Event_Card_ModifyInHand)
	{
		ACardBase* CallingCard = Cast<ACardBase>(CallingObject);
		COS_IF_CHECK_VOID(CallingCard, TEXT("CallingObject를 Card로 형변환에 실패했습니다"));

		if (CallingCard->GetGameplayTags().HasTagExact(CosGameTags::Effect_Attack))
		{
			if (CallingCard->GetOwner() == GetOwner())
			{
				CallingCard->ModifyCardEffectValues(StatusValue, ECardDataType::Hand, FGameplayTagContainer(), FGameplayTagContainer(CosGameTags::Effect_Attack));
			}
		}
	}
}
