#include "Utilities/CosGameplayTags.h"


namespace CosGameTags
{
	UE_DEFINE_GAMEPLAY_TAG(Target, TEXT("Target"))
	UE_DEFINE_GAMEPLAY_TAG(Target_Ally, TEXT("Target.Ally"))
	UE_DEFINE_GAMEPLAY_TAG(Target_Hero, TEXT("Target.Hero"))
	UE_DEFINE_GAMEPLAY_TAG(Target_Minion, TEXT("Target.Minion"))
	UE_DEFINE_GAMEPLAY_TAG(Target_Enemy, TEXT("Target.Enemy"))

	UE_DEFINE_GAMEPLAY_TAG(Event, TEXT("Event"))
	UE_DEFINE_GAMEPLAY_TAG(Event_CardSelectionMode, TEXT("Event.CardSelectionMode"))
	UE_DEFINE_GAMEPLAY_TAG(Event_GameOver, TEXT("Event.GameOver"))
	UE_DEFINE_GAMEPLAY_TAG(Event_Victory, TEXT("Event.Victory"))
	UE_DEFINE_GAMEPLAY_TAG(Event_TurnStart, TEXT("Event.TurnStart"))
	UE_DEFINE_GAMEPLAY_TAG(Event_Death, TEXT("Event.Death"))
	UE_DEFINE_GAMEPLAY_TAG(Event_CloseRewardScreen, TEXT("Event.CloseRewardScreen"))
	UE_DEFINE_GAMEPLAY_TAG(Event_Debug_DispathcerHubUpdate, TEXT("Event.Debug.DispathcerHubUpdate"))


	UE_DEFINE_GAMEPLAY_TAG(Event_Action, TEXT("Event_Action"))
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_AutoPlay, TEXT("Event.Action.AutoPlay"))
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_FlickCard, TEXT("Event.Action.FlickCard"))
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_GenerateCard, TEXT("Event.Action.GenerateCard"))
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_ResourceChange, TEXT("Event.Action.ResourceChange"))
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_ReturnCardToHand, TEXT("Event.Action.ReturnCardToHand"))
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_UseCard, TEXT("Event.Action.UseCard"))
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_Victory, TEXT("Event.Action.Victory"))


	UE_DEFINE_GAMEPLAY_TAG(Pile, TEXT("Pile"))
	UE_DEFINE_GAMEPLAY_TAG(Pile_Hand, TEXT("Pile.Hand"))
}
