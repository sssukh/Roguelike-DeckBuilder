#include "UI/UW_HeroSelect.h"

#include "Components/ComboBoxString.h"

void UUW_HeroSelect::GetSelectedData(FHeroDeck& OutHeroDeck)
{
	TArray<FName> HeroRowNames = HeroTable->GetRowNames();
	int32 SelectedHeroIndex = HeroSelector->FindOptionIndex(HeroSelector->GetSelectedOption());
	OutHeroDeck.Hero.DataTable = HeroTable;
	OutHeroDeck.Hero.RowName = HeroRowNames[SelectedHeroIndex];

	TArray<FName> DeckRowNames = DeckTable->GetRowNames();
	int32 SelectedDeckIndex = DeckSelector->FindOptionIndex(DeckSelector->GetSelectedOption());
	OutHeroDeck.Deck.DataTable = DeckTable;
	OutHeroDeck.Deck.RowName = DeckRowNames[SelectedDeckIndex];
}
