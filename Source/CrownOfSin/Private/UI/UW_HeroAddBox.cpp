#include "UI/UW_HeroAddBox.h"

#include "Components/ScrollBox.h"
#include "UI/UW_HeroSelect.h"

TArray<FHeroDeck> UUW_HeroAddBox::GetSelectedHeroesAndDecks()
{
	TArray<FHeroDeck> SelectedHeroesAndDecks;

	TArray<UWidget*> HeroSelectWidgets = HeroesBox->GetAllChildren();
	for (UWidget* Widget : HeroSelectWidgets)
	{
		UUW_HeroSelect* HeroSelect = Cast<UUW_HeroSelect>(Widget);
		if (!HeroSelect)
			continue;

		FHeroDeck HeroDeckData;
		HeroSelect->GetSelectedData(HeroDeckData);
		SelectedHeroesAndDecks.Add(HeroDeckData);
	}

	return SelectedHeroesAndDecks;
}

bool UUW_HeroAddBox::GetAddButtonIsEnabled()
{
	return HeroesBox->GetChildrenCount() < MaxHeroes;
}

bool UUW_HeroAddBox::GetSubtractButtonIsEnabled()
{
	return HeroesBox->GetChildrenCount() > MinHeroes;
}
