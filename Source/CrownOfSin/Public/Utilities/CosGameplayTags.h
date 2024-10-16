﻿#pragma once

#include "NativeGameplayTags.h"


namespace CosGameTags
{
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(None)

	// DT_TargetTags
	
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(None)
	
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Target)
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Target_Ally)
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Target_Hero)
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Target_Minion)
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Target_Enemy)
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Target_CardPlayer)
	
	// EventTags_Actions
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Action_ResourceChange);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Action_Draw);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Action_FlickCard);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Action_AutoPlay);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Action_NewPlayerTurn);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Action_UseCard);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Action_Victory);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Action_GameOver);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Action_GenerateCard);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Action_WinGame);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Action_ModifyStatusMax);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Action_ReturnCardToHand);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Action_Reshuffle);

	// EventTags_Card
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event)
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Card_Draw);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Card_Discard);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Card_Exhaust);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Card_Grab);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Card_GenerateCard);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Card_Drop);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Card_Void);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Card_Destroy);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Card_PrePlay);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Card_PostPlay);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Card_ModifyInHand);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Card_ReturnToHand);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Card_EnterHand);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Card_ExitHand);

	// FlagTags
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Flag_Active);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Flag_Passive);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Flag_AutoPlayed);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Flag_Specific);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Flag_Skippable);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Flag_DoNotClear);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Flag_FilterToAllowed);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Flag_KeepAfterAutoPlay);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Flag_Slow);

	// EventTags_Misc
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_TurnStart);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_TurnEnd);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_PreAttack);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_PostAttack);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_TargetSpawned);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_PreTakeDamage);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_PostTakeDamage);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_IncomingAttack);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_MouseRelease);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Debug_DispatcherHubUpdate);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Death);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_PreModifyStatus);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_PostModifyStatus);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_GameOver);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Victory);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_GameStart);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Test);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_WinGame);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_CardSelectionMode);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_CloseRewardScreen);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Reshuffle);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_ManaGain);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_DeckSetupDone);
	
	// DT_PileTags
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pile);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pile_Deck);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pile_Hand);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pile_Draw);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pile_Discard);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pile_Exhaust);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pile_Void);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pile_Shop);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pile_Destroy);


	// DT_EncounterTags
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Encounter);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Encounter_Easy);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Encounter_Normal);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Encounter_Elite);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Encounter_Boss);

	// DT_RarityTags
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Rarity);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Rarity_Invalid);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Rarity_Common);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Rarity_Rare);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Rarity_Epic);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Rarity_Legendary);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Rarity_Curse);

	// DT_InteractTags
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Interact);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Interact_Mark);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Interact_UnMark);

	// DT_PositionTags
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Position);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Position_Front);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Position_Back);

	// DT_StatusBarTags
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatusBar);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatusBar_MinionBar);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatusBar_ArtifactBar);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatusBar_RewardBar);

	// DT_AnimationTags
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Anim);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Anim_Attack);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Anim_Flinch);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Anim_Die);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Anim_Cast);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Anim_Block);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Anim_Spawn);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Anim_Idle);

	// DT_CardTags
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Attack);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Skill);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Power);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Defense);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Damage_Physical);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Damage_Magic);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Damage_Fire);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Damage_Poison);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Buff);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Debuff);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Invalid);
	CROWNOFSIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Curse);

}
