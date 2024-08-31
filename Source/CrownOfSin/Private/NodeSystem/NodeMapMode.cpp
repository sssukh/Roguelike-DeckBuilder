#include "NodeSystem/NodeMapMode.h"

#include "Interfaces/Interface_CardGameInstance.h"
#include "Kismet/GameplayStatics.h"

ANodeMapMode::ANodeMapMode(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void ANodeMapMode::BeginPlay()
{
	Super::BeginPlay();

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	
	IInterface_CardGameInstance::Execute_AttemptSaveGame(GameInstance, FString(), false);
}
