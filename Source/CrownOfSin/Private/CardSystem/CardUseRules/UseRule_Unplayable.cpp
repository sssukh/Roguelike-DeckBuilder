#include "CardSystem/CardUseRules/UseRule_Unplayable.h"


UUseRule_Unplayable::UUseRule_Unplayable()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UUseRule_Unplayable::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UUseRule_Unplayable::CheckIfUseAllowed(FUseRule UseRuleData, FString& FailMessage)
{
	FailMessage = FString(TEXT("Card is Unplayable"));
	return false;
}
