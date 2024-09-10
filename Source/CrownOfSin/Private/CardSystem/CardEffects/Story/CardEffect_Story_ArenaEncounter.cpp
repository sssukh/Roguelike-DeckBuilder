#include "CardSystem/CardEffects/Story/CardEffect_Story_ArenaEncounter.h"

#include "Interfaces/Interface_CardGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "NodeSystem/NodeEnumStruct.h"


// Sets default values for this component's properties
UCardEffect_Story_ArenaEncounter::UCardEffect_Story_ArenaEncounter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}


// Called when the game starts
void UCardEffect_Story_ArenaEncounter::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_Story_ArenaEncounter::ResolveCardEffect(AActor* TargetActor)
{
	FEncounterData* FoundEncounter = UsedData.DataTable->FindRow<FEncounterData>(UsedData.RowName,TEXT(""));
	if (!FoundEncounter)
		return false;

	FEncounterData EncounterData = *FoundEncounter;

	UGameInstance* CardGameInstance = UFunctionLibrary_Singletons::GetCardGameInstance(this);
	if (!CardGameInstance)
		return false;

	IInterface_CardGameInstance::Execute_AttemptSaveGame(CardGameInstance, FString(), true);
	IInterface_CardGameInstance::Execute_SetCurrentEncounterInInstance(CardGameInstance, EncounterData);

	UGameplayStatics::OpenLevel(this, FName(*EncounterData.Level));
	return true;
}
