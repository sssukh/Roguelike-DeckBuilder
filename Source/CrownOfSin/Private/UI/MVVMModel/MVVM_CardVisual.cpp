#include "UI/MVVMModel/MVVM_CardVisual.h"

#include "CardSystem/CardBase.h"
#include "StatusSystem/Status_Mana.h"

UMVVM_CardVisual::UMVVM_CardVisual()
{
}

void UMVVM_CardVisual::UpdateCardWidget()
{
	UpdateManaCrystal();

	CardFrameTint = CardActor->GetCardFrameTint(ECardDataType::Hand);
	FrameImage = CardActor->GetCardFrame(ECardDataType::Hand);

	CardImage = CardActor->GetCardPortrait(ECardDataType::Hand);
	CardText = CardActor->GetCardName(ECardDataType::Hand);
}

void UMVVM_CardVisual::SetManaCost(const int32& NewManaCost)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(ManaCost, NewManaCost))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(ManaCost);
	}
}

void UMVVM_CardVisual::SetManaCostColor(const FSlateColor& NewManaCostColor)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(ManaCostColor, NewManaCostColor))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(ManaCostColor);
	}
}

void UMVVM_CardVisual::UpdateManaCrystal()
{
	int32 HandUseRuleIndex;
	int32 HandManaCost;
	if (CardActor->GetStatusCostValueFromUseRules(ECardDataType::Hand, UStatus_Mana::StaticClass(), HandManaCost, HandUseRuleIndex))
	{
		SetManaCost(HandManaCost);

		int32 DeckUseRuleIndex;
		int32 DeckManaCost;
		if (CardActor->GetStatusCostValueFromUseRules(ECardDataType::Deck, UStatus_Mana::StaticClass(), DeckManaCost, DeckUseRuleIndex))
		{
			int32 CostConsumption = FMath::Sign<int32>(DeckManaCost - HandManaCost) + 1;

			if (CostConsumption == 0) SetManaCostColor(FSlateColor(FLinearColor(1, 0, 0, 1)));
			else if (CostConsumption == 1) SetManaCostColor(FSlateColor(FLinearColor(1, 1, 1, 1)));
			else if (CostConsumption == 2) SetManaCostColor(FSlateColor(FLinearColor(0, 1, 0, 1)));
		}
	}
	else
	{
		SetManaCost(0);
		SetManaCostColor(FSlateColor(FLinearColor(1, 0, 0, 1)));
	}
}
