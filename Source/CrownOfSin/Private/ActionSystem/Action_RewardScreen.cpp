#include "ActionSystem/Action_RewardScreen.h"

#include "CardSystem/CardPlayer.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "UI/UW_Layout_Cos.h"
#include "UI/UW_RewardScreen.h"


AAction_RewardScreen::AAction_RewardScreen()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AAction_RewardScreen::BeginPlay()
{
	Super::BeginPlay();
}

void AAction_RewardScreen::PlayAction_Implementation()
{
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	if (!CardPlayer) return;
	CardPlayer->PlayerUI->WBP_RewardScreen->Appear();
	Execute_EndAction(this);
}
