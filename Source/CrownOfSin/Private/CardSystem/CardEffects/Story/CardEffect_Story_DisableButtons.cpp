#include "CardSystem/CardEffects/Story/CardEffect_Story_DisableButtons.h"

#include "CardSystem/CardPlayer.h"
#include "Components/SizeBox.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "UI/UW_Layout_Cos.h"
#include "UI/UW_StoryEncounter.h"


// Sets default values for this component's properties
UCardEffect_Story_DisableButtons::UCardEffect_Story_DisableButtons()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCardEffect_Story_DisableButtons::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_Story_DisableButtons::ResolveCardEffect(AActor* TargetActor)
{
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	if (!CardPlayer) return false;

	UUW_StoryEncounter* WBP_StoryEncounter = Cast<UUW_StoryEncounter>(CardPlayer->PlayerUI->StoryEncounterBox->GetChildAt(0));
	if (!WBP_StoryEncounter) return false;

	WBP_StoryEncounter->DisableOptions();
	return true;
}
