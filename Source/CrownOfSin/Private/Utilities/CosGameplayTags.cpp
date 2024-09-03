#include "Utilities/CosGameplayTags.h"


namespace CosGameTags
{
	UE_DEFINE_GAMEPLAY_TAG(Target, TEXT("Target"))
	UE_DEFINE_GAMEPLAY_TAG(Target_Ally, TEXT("Target.Ally"))
	UE_DEFINE_GAMEPLAY_TAG(Target_Hero, TEXT("Target.Hero"))
	UE_DEFINE_GAMEPLAY_TAG(Target_Minion, TEXT("Target.Minion"))
	UE_DEFINE_GAMEPLAY_TAG(Target_Enemy, TEXT("Target.Enemy"))
	
	
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_ResourceChange,TEXT("Event.Action.ResourceChange"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_DrawAction,TEXT("Event.Action.Draw"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_FlickCardAction,TEXT("Event.Action.FlickCard"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_AutoPlayAction,TEXT("Event.Action.AutoPlay"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_NewPlayerTurnAction,TEXT("Event.Action.NewPlayerTurn"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_UseCardAction,TEXT("Event.Action.UseCard"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_VictoryAction,TEXT("Event.Action.Victory"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_GameOverAction,TEXT("Event.Action.GameOver"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_GenerateCardAction,TEXT("Event.Action.GenerateCard"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_WinGameAction,TEXT("Event.Action.WinGame"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_ModifyStatusMaxAction,TEXT("Event.Action.ModifyStatusMax"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_ReturnCardToHandAction,TEXT("Event.Action.ReturnCardToHand"));

	UE_DEFINE_GAMEPLAY_TAG(Event_Card_Draw, TEXT("Event.Card.Draw"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Card_Discard, TEXT("Event.Card.Discard"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Card_Exhaust, TEXT("Event.Card.Exhaust"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Card_Grab, TEXT("Event.Card.Grab"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Card_GenerateCard, TEXT("Event.Card.Generate"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Card_CardSelectionMode, TEXT("Event.CardSelectionMode"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Card_DropCard, TEXT("Event.Card.Drop"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Card_Void, TEXT("Event.Card.Void"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Card_Destroy, TEXT("Event.Card.Destroy"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Card_PrePlay, TEXT("Event.Card.PrePlay"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Card_PostPlay, TEXT("Event.Card.PostPlay"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Card_ModifyInHand, TEXT("Event.Card.ModifyInHand"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Card_ReturnToHand, TEXT("Event.Card.ReturnToHand"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Card_EnterHand, TEXT("Event.Card.EnterHand"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Card_ExitHand, TEXT("Event.Card.ExitHand"));

	UE_DEFINE_GAMEPLAY_TAG(Flag_Active, TEXT("Flag.Active"));
	UE_DEFINE_GAMEPLAY_TAG(Flag_Passive, TEXT("Flag.Passive"));
	UE_DEFINE_GAMEPLAY_TAG(Flag_AutoPlayed, TEXT("Flag.AutoPlayed"));
	UE_DEFINE_GAMEPLAY_TAG(Flag_Specific, TEXT("Flag.Specific"));
	UE_DEFINE_GAMEPLAY_TAG(Flag_Skippable, TEXT("Flag.Skippable"));
	UE_DEFINE_GAMEPLAY_TAG(Flag_DoNotClear, TEXT("Flag.DoNotClear"));
	UE_DEFINE_GAMEPLAY_TAG(Flag_FilterToAllowed, TEXT("Flag.FilterToAllowed"));
	UE_DEFINE_GAMEPLAY_TAG(Flag_KeepAfterAutoPlay, TEXT("Flag.KeepAfterAutoPlay"));
	UE_DEFINE_GAMEPLAY_TAG(Flag_Slow, TEXT("Flag.Slow"));

	UE_DEFINE_GAMEPLAY_TAG(Event_TurnStart, TEXT("Event.TurnStart"));
	UE_DEFINE_GAMEPLAY_TAG(Event_TurnEnd, TEXT("Event.TurnEnd"));
	UE_DEFINE_GAMEPLAY_TAG(Event_PreAttack, TEXT("Event.PreAttack"));
	UE_DEFINE_GAMEPLAY_TAG(Event_PostAttack, TEXT("Event.PostAttack"));
	UE_DEFINE_GAMEPLAY_TAG(Event_TargetSpawned, TEXT("Event.TargetSpawned"));
	UE_DEFINE_GAMEPLAY_TAG(Event_PreTakeDamage, TEXT("Event.PreTakeDamage"));
	UE_DEFINE_GAMEPLAY_TAG(Event_PostTakeDamage, TEXT("Event.PostTakeDamage"));
	UE_DEFINE_GAMEPLAY_TAG(Event_IncomingAttack, TEXT("Event.IncomingAttack"));
	UE_DEFINE_GAMEPLAY_TAG(Event_MouseRelease, TEXT("Event.MouseRelease"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Debug_DispatcherHubUpdate, TEXT("Event.Debug.DispatcherHubUpdate"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Death, TEXT("Event.Death"));
	UE_DEFINE_GAMEPLAY_TAG(Event_PreModifyStatus, TEXT("Event.PreModifyStatus"));
	UE_DEFINE_GAMEPLAY_TAG(Event_PostModifyStatus, TEXT("Event.PostModifyStatus"));
	UE_DEFINE_GAMEPLAY_TAG(Event_GameOver, TEXT("Event.GameOver"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Victory, TEXT("Event.Victory"));
	UE_DEFINE_GAMEPLAY_TAG(Event_GameStart, TEXT("Event.GameStart"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Test, TEXT("Event.Test"));
	UE_DEFINE_GAMEPLAY_TAG(Event_WinGame, TEXT("Event.WinGame"));
	UE_DEFINE_GAMEPLAY_TAG(Event_CardSelectionMode, TEXT("Event.CardSelectionMode"));
	UE_DEFINE_GAMEPLAY_TAG(Event_CloseRewardScreen, TEXT("Event.CloseRewardScreen"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Reshuffle, TEXT("Event.Reshuffle"));
	UE_DEFINE_GAMEPLAY_TAG(Event_ManaGain, TEXT("Event.ManaGain"));
	UE_DEFINE_GAMEPLAY_TAG(Event_DeckSetupDone, TEXT("Event.DeckSetupDone"));





}
