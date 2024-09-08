#include "CardSystem/CardEffects/CardEffect_RevealShop.h"

#include "CardSystem/CardPlayer.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "UI/UW_Layout_Cos.h"
#include "UI/UW_Shop.h"


// Sets default values for this component's properties
UCardEffect_RevealShop::UCardEffect_RevealShop()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCardEffect_RevealShop::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_RevealShop::ResolveCardEffect(AActor* TargetActor)
{
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	if (!CardPlayer)
		return false;

	CardPlayer->PlayerUI->WBP_Shop->UpdateShopCards();
	CardPlayer->PlayerUI->WBP_Shop->SetVisibility(ESlateVisibility::Visible);
	return true;
}
