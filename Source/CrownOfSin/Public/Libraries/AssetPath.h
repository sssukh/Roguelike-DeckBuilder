﻿#pragma once


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

