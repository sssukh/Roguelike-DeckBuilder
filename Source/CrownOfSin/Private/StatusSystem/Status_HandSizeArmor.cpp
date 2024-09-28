#include "StatusSystem/Status_HandSizeArmor.h"

#include "CardSystem/CardPlayer.h"
#include "CardSystem/Piles/PileHandComponent.h"
#include "Core/DispatcherHubLocalComponent.h"
#include "Libraries/AssetPath.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "StatusSystem/Status_Armor.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


// Sets default values for this component's properties
UStatus_HandSizeArmor::UStatus_HandSizeArmor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	static ConstructorHelpers::FObjectFinder<UTexture2D> T_DiceShield(*AssetPath::Texture::T_DiceShield);
	if (T_DiceShield.Succeeded())
	{
		Icon = T_DiceShield.Object;
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("T_DiceShield를 로드하지 못했습니다."));
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Tooltips_Statuses(*AssetPath::DataTable::DT_Tooltips_Statuses);
	if (DT_Tooltips_Statuses.Succeeded())
	{
		FToolTipValue NewToolTipValue;
		NewToolTipValue.ToolTipTable.DataTable = DT_Tooltips_Statuses.Object;
		NewToolTipValue.ToolTipTable.RowName = FName(TEXT("ShieldHand"));
		NewToolTipValue.bValued = false;
		Tooltips.Add(NewToolTipValue);
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("DT_Tooltips_Statuses를 로드하지 못했습니다"));
	}

	Tint = FLinearColor(1, 1, 1, 1);
	
	Priority = 5.0f;
	FriendlyName = FText::FromString(TEXT("Shield Hand"));
	GameplayTags = FGameplayTagContainer(CosGameTags::Effect_Buff);
}


// Called when the game starts
void UStatus_HandSizeArmor::BeginPlay()
{
	Super::BeginPlay();

	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

	CardPlayer->DispatcherHubLocalComponent->BindEventToHub(this, CosGameTags::Event_TurnEnd);
}

void UStatus_HandSizeArmor::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	if (EventTag != CosGameTags::Event_TurnEnd)
	{
		return;
	}

	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

	UPileHandComponent* PileHandComponent = CardPlayer->FindComponentByClass<UPileHandComponent>();

	if (PileHandComponent->Cards.Num() > 0)
	{
		if (GetOwner()->GetClass()->ImplementsInterface(UInterface_CardTarget::StaticClass()))
		{
			IInterface_CardTarget::Execute_AddToStatus(GetOwner(), UStatus_Armor::StaticClass(), PileHandComponent->Cards.Num(), true, nullptr);
		}
	}
}
