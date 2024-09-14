// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusSystem/Status_HandSizeArmor.h"

#include "CardSystem/CardPlayer.h"
#include "CardSystem/Piles/PileHandComponent.h"
#include "Core/DispatcherHubLocalComponent.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "StatusSystem/Status_Armor.h"
#include "Utilities/CosGameplayTags.h"


// Sets default values for this component's properties
UStatus_HandSizeArmor::UStatus_HandSizeArmor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatus_HandSizeArmor::BeginPlay()
{
	Super::BeginPlay();

	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

	CardPlayer->DispatcherHubLocalComponent->BindEventToHub(this,CosGameTags::Event_TurnEnd);
	
}

void UStatus_HandSizeArmor::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject,
	bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	if(EventTag!=CosGameTags::Event_TurnEnd)
	{
		return;
	}

	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

	UPileHandComponent* PileHandComponent = CardPlayer->FindComponentByClass<UPileHandComponent>();

	if(PileHandComponent->Cards.Num()>0)
	{
		if(GetOwner()->GetClass()->ImplementsInterface(UInterface_CardTarget::StaticClass()))
		{
			IInterface_CardTarget::Execute_AddToStatus(GetOwner(),UStatus_Armor::StaticClass(),PileHandComponent->Cards.Num(),true,nullptr);
		}
	}
}




