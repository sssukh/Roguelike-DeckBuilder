// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusSystem/Status_Mana.h"

#include "Libraries/FunctionLibrary_Event.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "Utilities/CosGameplayTags.h"


// Sets default values for this component's properties
UStatus_Mana::UStatus_Mana()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

int32 UStatus_Mana::AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon,
	bool bRefreshAppearance, UObject* InPayload)
{
	UFunctionLibrary_Event::QueueEventInGlobalDispatcherHub(CosGameTags::Event_Action_ResourceChange,this);
	
	return Super::AddStatusValue(InAmount, bShowSplashNumber, bShowSplashIcon, bRefreshAppearance, InPayload);
}




