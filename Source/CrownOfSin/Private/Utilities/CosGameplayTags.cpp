#include "Utilities/CosGameplayTags.h"


namespace CosGameTags
{
	UE_DEFINE_GAMEPLAY_TAG(None, TEXT("None"))
	
	UE_DEFINE_GAMEPLAY_TAG(Target, TEXT("Target"))
	UE_DEFINE_GAMEPLAY_TAG(Target_Ally, TEXT("Target.Ally"))
	UE_DEFINE_GAMEPLAY_TAG(Target_Hero, TEXT("Target.Hero"))
	UE_DEFINE_GAMEPLAY_TAG(Target_Minion, TEXT("Target.Minion"))
	UE_DEFINE_GAMEPLAY_TAG(Target_Enemy, TEXT("Target.Enemy"))
	UE_DEFINE_GAMEPLAY_TAG(Target_CardPlayer, TEXT("Target.CardPlayer"))

	
	
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_ResourceChange,TEXT("Event.Action.ResourceChange"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_Draw,TEXT("Event.Action.Draw"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_FlickCard,TEXT("Event.Action.FlickCard"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_AutoPlay,TEXT("Event.Action.AutoPlay"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_NewPlayerTurn,TEXT("Event.Action.NewPlayerTurn"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_UseCard,TEXT("Event.Action.UseCard"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_Victory,TEXT("Event.Action.Victory"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_GameOver,TEXT("Event.Action.GameOver"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_GenerateCard,TEXT("Event.Action.GenerateCard"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_WinGame,TEXT("Event.Action.WinGame"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_ModifyStatusMax,TEXT("Event.Action.ModifyStatusMax"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_ReturnCardToHand,TEXT("Event.Action.ReturnCardToHand"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Action_Reshuffle,TEXT("Event.Action.Reshuffle"));

	UE_DEFINE_GAMEPLAY_TAG(Event_Card_Draw, TEXT("Event.Card.Draw"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Card_Discard, TEXT("Event.Card.Discard"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Card_Exhaust, TEXT("Event.Card.Exhaust"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Card_Grab, TEXT("Event.Card.Grab"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Card_GenerateCard, TEXT("Event.Card.Generate"));
	UE_DEFINE_GAMEPLAY_TAG(Event_Card_Drop, TEXT("Event.Card.Drop"));
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
	
	UE_DEFINE_GAMEPLAY_TAG(Event, TEXT("Event"))

	UE_DEFINE_GAMEPLAY_TAG(Pile, TEXT("Pile"))
	UE_DEFINE_GAMEPLAY_TAG(Pile_Deck, TEXT("Pile.Deck"))
	UE_DEFINE_GAMEPLAY_TAG(Pile_Hand, TEXT("Pile.Hand"))
	UE_DEFINE_GAMEPLAY_TAG(Pile_Draw, TEXT("Pile.Draw"))
	UE_DEFINE_GAMEPLAY_TAG(Pile_Discard, TEXT("Pile.Discard"))
	UE_DEFINE_GAMEPLAY_TAG(Pile_Exhaust, TEXT("Pile.Exhaust"))
	UE_DEFINE_GAMEPLAY_TAG(Pile_Void, TEXT("Pile.Void"))
	UE_DEFINE_GAMEPLAY_TAG(Pile_Shop, TEXT("Pile.Shop"))
	UE_DEFINE_GAMEPLAY_TAG(Pile_Destroy, TEXT("Pile.Destroy"))


	UE_DEFINE_GAMEPLAY_TAG(Encounter, TEXT("Encounter"))
	UE_DEFINE_GAMEPLAY_TAG(Encounter_Easy, TEXT("Encounter.Easy"))
	UE_DEFINE_GAMEPLAY_TAG(Encounter_Normal, TEXT("Encounter.Normal"))
	UE_DEFINE_GAMEPLAY_TAG(Encounter_Elite, TEXT("Encounter.Elite"))
	UE_DEFINE_GAMEPLAY_TAG(Encounter_Boss, TEXT("Encounter.Boss"))

	UE_DEFINE_GAMEPLAY_TAG(Rarity, TEXT("Rarity"))
	UE_DEFINE_GAMEPLAY_TAG(Rarity_Invalid, TEXT("Rarity.Invalid"))
	UE_DEFINE_GAMEPLAY_TAG(Rarity_Common, TEXT("Rarity.Common"))
	UE_DEFINE_GAMEPLAY_TAG(Rarity_Rare, TEXT("Rarity.Rare"))
	UE_DEFINE_GAMEPLAY_TAG(Rarity_Epic, TEXT("Rarity.Epic"))
	UE_DEFINE_GAMEPLAY_TAG(Rarity_Legendary, TEXT("Rarity.Legendary"))
	UE_DEFINE_GAMEPLAY_TAG(Rarity_Curse, TEXT("Rarity.Curse"))

	UE_DEFINE_GAMEPLAY_TAG(Interact, TEXT("Interact"))
	UE_DEFINE_GAMEPLAY_TAG(Interact_Mark, TEXT("Interact.Mark"))
	UE_DEFINE_GAMEPLAY_TAG(Interact_UnMark, TEXT("Interact.Unmark"))

	UE_DEFINE_GAMEPLAY_TAG(Position, TEXT("Position"))
	UE_DEFINE_GAMEPLAY_TAG(Position_Front, TEXT("Position.Front"))
	UE_DEFINE_GAMEPLAY_TAG(Position_Back, TEXT("Position.Back"))

	UE_DEFINE_GAMEPLAY_TAG(StatusBar, TEXT("StatusBar"))
	UE_DEFINE_GAMEPLAY_TAG(StatusBar_MinionBar, TEXT("StatusBar.MinionBar"))
	UE_DEFINE_GAMEPLAY_TAG(StatusBar_ArtifactBar, TEXT("StatusBar.ArtifactBar"))
	UE_DEFINE_GAMEPLAY_TAG(StatusBar_RewardBar, TEXT("StatusBar.RewardBar"))

	UE_DEFINE_GAMEPLAY_TAG(Anim, TEXT("Anim"))
	UE_DEFINE_GAMEPLAY_TAG(Anim_Attack, TEXT("Anim.Attack"))
	UE_DEFINE_GAMEPLAY_TAG(Anim_Flinch, TEXT("Anim.Flinch"))
	UE_DEFINE_GAMEPLAY_TAG(Anim_Die, TEXT("Anim.Die"))
	UE_DEFINE_GAMEPLAY_TAG(Anim_Cast, TEXT("Anim.Cast"))
	UE_DEFINE_GAMEPLAY_TAG(Anim_Block, TEXT("Anim.Block"))
	UE_DEFINE_GAMEPLAY_TAG(Anim_Spawn, TEXT("Anim.Spawn"))
	UE_DEFINE_GAMEPLAY_TAG(Anim_Idle, TEXT("Anim.Idle"))

	UE_DEFINE_GAMEPLAY_TAG(Effect, TEXT("Effect"))
	UE_DEFINE_GAMEPLAY_TAG(Effect_Attack, TEXT("Effect.Attack"))
	UE_DEFINE_GAMEPLAY_TAG(Effect_Skill, TEXT("Effect.Skill"))
	UE_DEFINE_GAMEPLAY_TAG(Effect_Power, TEXT("Effect.Power"))
	UE_DEFINE_GAMEPLAY_TAG(Effect_Defense, TEXT("Effect.Defense"))
	UE_DEFINE_GAMEPLAY_TAG(Effect_Damage_Physical, TEXT("Effect.Damage.Physical"))
	UE_DEFINE_GAMEPLAY_TAG(Effect_Damage_Magic, TEXT("Effect.Damage.Magic"))
	UE_DEFINE_GAMEPLAY_TAG(Effect_Damage_Fire, TEXT("Effect.Damage.Fire"))
	UE_DEFINE_GAMEPLAY_TAG(Effect_Damage_Poison, TEXT("Effect.Damage.Poison"))
	UE_DEFINE_GAMEPLAY_TAG(Effect_Buff, TEXT("Effect.Buff"))
	UE_DEFINE_GAMEPLAY_TAG(Effect_Debuff, TEXT("Effect.Debuff"))
	UE_DEFINE_GAMEPLAY_TAG(Effect_Invalid, TEXT("Effect.Invalid"))
	UE_DEFINE_GAMEPLAY_TAG(Effect_Curse, TEXT("Effect.Curse"))

}
