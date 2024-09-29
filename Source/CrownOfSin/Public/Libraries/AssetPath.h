#pragma once


// 에셋 경로를 중앙에서 관리하는 네임스페이스
namespace AssetPath
{
	namespace Blueprint
	{
		const FString BP_GlobalDispatcherHub_C = TEXT("Blueprint'/Game/CrownOfSin/BluePrints/Core/BP_GlobalDispatcherHub.BP_GlobalDispatcherHub_C'");
		const FString BP_CardPlayer_C = TEXT("Blueprint'/Game/CrownOfSin/BluePrints/Core/BP_CardPlayer.BP_CardPlayer_C'");
		const FString BP_RewardHolder_C = TEXT("Blueprint'/Game/CrownOfSin/BluePrints/Core/BP_RewardHolder.BP_RewardHolder_C'");
		
		const FString BP_TurnManager_C = TEXT("Blueprint'/Game/CrownOfSin/BluePrints/Core/BP_TurnManager.BP_TurnManager_C'");

		const FString BP_ScreenShake_C = TEXT("Blueprint'/Game/CrownOfSin/BluePrints/UI/BP_ScreenShake.BP_ScreenShake_C'");
		const FString BP_IconSplash_C = TEXT("Blueprint'/Game/CrownOfSin/BluePrints/UI/BP_IconSplash.BP_IconSplash_C'");
		const FString BP_TextSplash_C = TEXT("Blueprint'/Game/CrownOfSin/BluePrints/UI/BP_TextSplash.BP_TextSplash_C'");


		const FString WBP_COS_Layout_C = TEXT("WidgetBlueprint'/Game/CrownOfSin/BluePrints/Widgets/WBP_COS_Layout.WBP_COS_Layout_C'");
		const FString WBP_ToolTip_C = TEXT("WidgetBlueprint'/Game/CrownOfSin/BluePrints/Widgets/WBP_Tooltip.WBP_Tooltip_C'");

		const FString WBP_ToolTipList_C = TEXT("WidgetBlueprint'/Game/CrownOfSin/BluePrints/Widgets/WBP_TooltipList.WBP_TooltipList_C'");

		const FString WBP_ScreenFade_C = TEXT("WidgetBlueprint'/Game/CrownOfSin/BluePrints/Widgets/WBP_ScreenFade.WBP_ScreenFade_C'");
		const FString WBP_StatusIcon_C = TEXT("WidgetBlueprint'/Game/CrownOfSin/BluePrints/Widgets/WBP_StatusIcon.WBP_StatusIcon_C'");
		const FString WBP_TargetingBezierPoint_C = TEXT("WidgetBlueprint'/Game/CrownOfSin/BluePrints/Widgets/WBP_TargetingBezierPoint.WBP_TargetingBezierPoint_C'");
		const FString WBP_MinionUiTop_C = TEXT("WidgetBlueprint'/Game/CrownOfSin/BluePrints/Widgets/WBP_MinionUiTop.WBP_MinionUiTop_C'");
		const FString WBP_MinionUI_C = TEXT("WidgetBlueprint'/Game/CrownOfSin/BluePrints/Widgets/WBP_MinionUI.WBP_MinionUI_C'");


		const FString WBP_CardVisual_C = TEXT("/Game/CrownOfSin/BluePrints/Widgets/WBP_CardVisual.WBP_CardVisual_C");
		const FString WBP_CardListCard_C = TEXT("WidgetBlueprint'/Game/CrownOfSin/BluePrints/Widgets/WBP_CardListCard.WBP_CardListCard_C'");
		const FString WBP_ShopCard_C = TEXT("WidgetBlueprint'/Game/CrownOfSin/BluePrints/Widgets/WBP_ShopCard.WBP_ShopCard_C'");
		const FString WBP_CardList_C = TEXT("WidgetBlueprint'/Game/CrownOfSin/BluePrints/Widgets/WBP_CardList.WBP_CardList_C'");
	}

	namespace Texture
	{
		const FString T_TargetArrow = TEXT("Texture2D'/Game/RLDB/Demo/Textures/UI/T_TargetArrow.T_TargetArrow'");
		const FString T_RandomDice = TEXT("Texture2D'/Game/RLDB/Demo/Textures/NodeMap/T_RandomDice.T_RandomDice'");
		const FString T_CrossMark = TEXT("Texture2D'/Game/RLDB/Demo/Textures/NodeMap/T_CrossMark.T_CrossMark'");
		const FString T_Transparent = TEXT("Texture2D'/Game/RLDB/Demo/Textures/Card/T_Transparent.T_Transparent'");
		const FString T_Coins = TEXT("Texture2D'/Game/RLDB/Demo/Textures/UI/T_Coins.T_Coins'");
		const FString T_LuchadorMask = TEXT("Texture2D'/Game/RLDB/Demo/Textures/Artifacts/T_LuchadorMask.T_LuchadorMask'");
		const FString T_Dagger = TEXT("Texture2D'/Game/RLDB/Demo/Textures/Statuses/T_Dagger.T_Dagger'");
		
		const FString T_Heart = TEXT("Texture2D'/Game/RLDB/Demo/Textures/UI/T_Heart.T_Heart'");
		const FString T_Shield_Metal = TEXT("Texture2D'/Game/RLDB/Demo/Textures/Statuses/T_Shield_Metal.T_Shield_Metal'");
		const FString T_Barrier = TEXT("Texture2D'/Game/RLDB/Demo/Textures/Statuses/T_Barrier.T_Barrier'");
		const FString T_Cursed = TEXT("Texture2D'/Game/RLDB/Demo/Textures/Statuses/T_Cursed.T_Cursed'");
		const FString T_Owl = TEXT("Texture2D'/Game/RLDB/Demo/Textures/Statuses/T_Owl.T_Owl'");
		const FString T_Fear = TEXT("Texture2D'/Game/RLDB/Demo/Textures/Statuses/T_Fear.T_Fear'");
		const FString T_King = TEXT("Texture2D'/Game/RLDB/Demo/Textures/Statuses/T_King.T_King'");
		const FString T_DiceShield = TEXT("Texture2D'/Game/RLDB/Demo/Textures/Statuses/T_DiceShield.T_DiceShield'");
		const FString T_Poison = TEXT("Texture2D'/Game/RLDB/Demo/Textures/Statuses/T_Poison.T_Poison'");		
		const FString T_Skittish = TEXT("Texture2D'/Game/RLDB/Demo/Textures/Statuses/T_Skittish.T_Skittish'");
		const FString T_Strength = TEXT("Texture2D'/Game/RLDB/Demo/Textures/Statuses/T_Strength.T_Strength'");
		const FString T_Venomous = TEXT("Texture2D'/Game/RLDB/Demo/Textures/Statuses/T_Venomous.T_Venomous'");
		const FString T_CardReward = TEXT("Texture2D'/Game/RLDB/Demo/Textures/Artifacts/T_CardReward.T_CardReward'");
		const FString T_Chest = TEXT("Texture2D'/Game/RLDB/Demo/Textures/NodeMap/T_Chest.T_Chest'");
		const FString T_Lollipop = TEXT("Texture2D'/Game/RLDB/Demo/Textures/Artifacts/T_Lollipop.T_Lollipop'");
		const FString T_HeavyTome = TEXT("Texture2D'/Game/RLDB/Demo/Textures/Artifacts/T_HeavyTome.T_HeavyTome'");
		const FString T_BagOfHolding = TEXT("Texture2D'/Game/RLDB/Demo/Textures/Artifacts/T_BagOfHolding.T_BagOfHolding'");
		const FString T_BootsOfSpeed = TEXT("Texture2D'/Game/RLDB/Demo/Textures/Artifacts/T_BootsOfSpeed.T_BootsOfSpeed'");
		const FString T_BrokenShield = TEXT("Texture2D'/Game/RLDB/Demo/Textures/Artifacts/T_BrokenShield.T_BrokenShield'");
		const FString T_CardReward = TEXT("Texture2D'/Game/RLDB/Demo/Textures/Artifacts/T_CardReward.T_CardReward'");
		const FString T_JackOLantern = TEXT("Texture2D'/Game/RLDB/Demo/Textures/Artifacts/T_JackOLantern.T_JackOLantern'");
		const FString T_RobesOfProtection = TEXT("Texture2D'/Game/RLDB/Demo/Textures/Artifacts/T_RobesOfProtection.T_RobesOfProtection'");
		const FString T_WaterSkin = TEXT("Texture2D'/Game/RLDB/Demo/Textures/Artifacts/T_Waterskin.T_Waterskin'");

	}

	namespace DataTable
	{
		const FString DT_StoryEncounters = TEXT("DataTable'/Game/CrownOfSin/Data/NodeMap/DT_StoryEncounters.DT_StoryEncounters'");
		const FString DT_Cards_Rewards = TEXT("DataTable'/Game/CrownOfSin/Data/Cards/DT_Cards_Rewards.DT_Cards_Rewards'");
		const FString DT_Artifacts = TEXT("DataTable'/Game/CrownOfSin/Data/DT_Artifacts.DT_Artifacts'");
		const FString DT_Tooltips_Statuses = TEXT("DataTable'/Game/CrownOfSin/Data/ToolTips/DT_Tooltips_Statuses.DT_Tooltips_Statuses'");
		const FString DT_RarityWeights = TEXT("DataTable'/Game/CrownOfSin/Data/DT_RarityWeights.DT_RarityWeights'");
		const FString DT_Tooltips_Artifacts = TEXT("DataTable'/Game/CrownOfSin/Data/DT_Tooltips_Artifacts.DT_Tooltips_Artifacts'");
	}

	namespace Meshes
	{
		const FString SM_Plane = TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'");
		const FString SM_Edge = TEXT("StaticMesh'/Game/RLDB/Demo/Meshes/SM_Edge.SM_Edge'");
	}

	namespace Material
	{
		const FString M_Node = TEXT("Material'/Game/RLDB/Demo/Materials/M_Node.M_Node'");
	}

	namespace Curve
	{
		const FString Curve_Decelerate_Float = TEXT("CurveFloat'/Game/RLDB/Blueprints/Curves/Curve_Decelerate_Float.Curve_Decelerate_Float'");
		const FString Curve_Parabel_Float = TEXT("CurveFloat'/Game/RLDB/Blueprints/Curves/Curve_Parabel_Float.Curve_Parabel_Float'");
	}

	// 필요한 다른 에셋 경로도 유사하게 추가
}
