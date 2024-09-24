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

		const FString WBP_ToolTip_C = TEXT("WidgetBlueprint'/Game/CrownOfSin/BluePrints/Widgets/WBP_Tooltip.WBP_Tooltip_C'");
		const FString WBP_ToolTipList_C = TEXT("WidgetBlueprint'/Game/CrownOfSin/BluePrints/Widgets/WBP_TooltipList.WBP_TooltipList_C'");
		const FString WBP_ScreenFade_C = TEXT("WidgetBlueprint'/Game/CrownOfSin/BluePrints/Widgets/WBP_ScreenFade.WBP_ScreenFade_C'");
		const FString WBP_StatusIcon_C = TEXT("WidgetBlueprint'/Game/CrownOfSin/BluePrints/Widgets/WBP_StatusIcon.WBP_StatusIcon_C'");
		const FString WBP_TargetingBezierPoint_C = TEXT("WidgetBlueprint'/Game/CrownOfSin/BluePrints/Widgets/WBP_TargetingBezierPoint.WBP_TargetingBezierPoint_C'");

		const FString WBP_MinionUiTop = TEXT("WidgetBlueprint'/Game/CrownOfSin/BluePrints/Widgets/WBP_MinionUiTop.WBP_MinionUiTop_C'");

		const FString WBP_CardVisual = TEXT("/Game/CrownOfSin/BluePrints/Widgets/WBP_CardVisual.WBP_CardVisual_C");
	}

	namespace Texture
	{
		const FString T_TargetArrow = TEXT("Texture2D'/Game/RLDB/Demo/Textures/UI/T_TargetArrow.T_TargetArrow'");
		const FString T_RandomDice = TEXT("Texture2D'/Game/RLDB/Demo/Textures/NodeMap/T_RandomDice.T_RandomDice'");
		const FString T_CrossMark = TEXT("Texture2D'/Game/RLDB/Demo/Textures/NodeMap/T_CrossMark.T_CrossMark'");
		const FString T_Transparent = TEXT("Texture2D'/Game/RLDB/Demo/Textures/Card/T_Transparent.T_Transparent'");
		const FString T_Coins = TEXT("Texture2D'/Game/RLDB/Demo/Textures/UI/T_Coins.T_Coins'");
		const FString T_LuchadorMask = TEXT("Texture2D'/Game/RLDB/Demo/Textures/Artifacts/T_LuchadorMask.T_LuchadorMask'");
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

	// 필요한 다른 에셋 경로도 유사하게 추가
}


// #include "CoreMinimal.h"
// #include "Engine/DataTable.h"
// #include "Utilities/CosLog.h"
// #include "UObject/ConstructorHelpers.h"
// #include "UObject/Object.h"
// #include "UObject/SoftObjectPath.h"
// #include "AssetTableRef.generated.h"


// 예를 들어, 어떤 자산을 로드하려면 다음과 같이 합니다.
/*
UTexture2D* MyTexture = FAssetReferenceUtility::LoadAssetFromDataTable(
	AssetRefPath::TexturesPath, FName("MyTextureRowName"));

// 클래스를 찾으려면 다음과 같이 합니다.
TSubclassOf<AActor> MyActorClass = FAssetReferenceUtility::FindClassFromDataTable<AActor>(
	AssetRefPath::BluePrintPath, FName("MyActorRowName"));
*/


// 자산 경로 상수 정의
// namespace AssetRefPath
// {
// 	const FString MeshesPath = TEXT("DataTable'/Game/CrownOfSin/AssetRef/DT_StaticMesh.DT_StaticMesh'");
// 	const FString MaterialPath = TEXT("DataTable'/Game/CrownOfSin/AssetRef/DT_Materials.DT_Materials'");
// 	const FString BluePrintPath = TEXT("DataTable'/Game/CrownOfSin/AssetRef/DT_BluePrints.DT_BluePrints'");
// 	const FString DataTablePath = TEXT("DataTable'/Game/CrownOfSin/AssetRef/DT_DataTable.DT_DataTable'");
// 	const FString TexturesPath = TEXT("DataTable'/Game/CrownOfSin/AssetRef/DT_Texture.DT_Texture'");
// }
//

//
// USTRUCT(BlueprintType)
// struct FAssetReferenceTableRow : public FTableRowBase
// {
// 	GENERATED_BODY()
//
// 	UPROPERTY(EditAnywhere, BlueprintReadOnly)
// 	TSoftObjectPtr<UObject> AssetReference;
// };
//
// /*DataTable 및 자산을 사용하는 유틸리티 기능을 제공합니다.*/
// class FAssetReferenceUtility
// {
// public:
// 	// 데이터 테이블 로드 함수
// 	static UDataTable* LoadDataTable(const FName& RowName)
// 	{
// 		return LoadAssetFromDataTable<UDataTable>(AssetRefPath::DataTablePath, RowName);
// 	}
//
// 	// 텍스처 로드 함수
// 	static UTexture2D* LoadTexture(const FName& RowName)
// 	{
// 		return LoadAssetFromDataTable<UTexture2D>(AssetRefPath::TexturesPath, RowName);
// 	}
//
// 	// 머티리얼 로드 함수
// 	static UMaterialInterface* LoadMaterial(const FName& RowName)
// 	{
// 		return LoadAssetFromDataTable<UMaterialInterface>(AssetRefPath::MaterialPath, RowName);
// 	}
//
// 	// 메쉬 로드 함수
// 	static UStaticMesh* LoadMesh(const FName& RowName)
// 	{
// 		return LoadAssetFromDataTable<UStaticMesh>(AssetRefPath::MeshesPath, RowName);
// 	}
//
// 	// 블루프린트 클래스 로드 함수
// 	template <typename T>
// 	static TSubclassOf<T> LoadBlueprintClass(const FName& RowName)
// 	{
// 		return FindClassFromDataTable<T>(AssetRefPath::BluePrintPath, RowName, true);
// 	}
//
// public:
// 	// 지정된 행 이름을 사용하여 지정된 DataTable에서 자산을 로드합니다.
// 	template <typename T>
// 	static T* LoadAssetFromDataTable(const FString& DataTablePath, const FName& RowName)
// 	{
// 		FString AssetPath;
// 		if (!IsInConstructorContext() || !TryGetAssetPathFromDataTable(DataTablePath, RowName, AssetPath))
// 		{
// 			return nullptr;
// 		}
//
// 		return LoadAsset<T>(AssetPath);
// 	}
//
// 	// DataTable의 행 이름을 사용하여 T 유형의 클래스를 찾습니다.
// 	template <typename T>
// 	static TSubclassOf<T> FindClassFromDataTable(const FString& DataTablePath, const FName& RowName, bool bIsBlueprint = false)
// 	{
// 		FString AssetPath;
// 		if (!IsInConstructorContext() || !TryGetAssetPathFromDataTable(DataTablePath, RowName, AssetPath))
// 		{
// 			return nullptr;
// 		}
//
// 		if (bIsBlueprint && !AssetPath.EndsWith("_C"))
// 		{
// 			AssetPath += TEXT("_C");
// 		}
//
// 		return FindClass<T>(AssetPath);
// 	}
//
// public:
// 	// 현재 코드 실행이 생성자 컨텍스트 내에 있는지 확인합니다.
// 	static bool IsInConstructorContext()
// 	{
// 		const FUObjectThreadContext& ThreadContext = FUObjectThreadContext::Get();
// 		if (ThreadContext.IsInConstructor > 0)
// 		{
// 			return true;
// 		}
//
// 		COS_LOG_ERROR(TEXT("어설션 실패: 생성자 외부에서 호출되었습니다."));
// 		return false;
// 	}
//
// 	// DataTable에서 자산 경로를 가져옵니다.
// 	static bool TryGetAssetPathFromDataTable(const FString& DataTablePath, const FName& RowName, FString& OutAssetPath)
// 	{
// 		const UDataTable* DataTable = LoadObject<UDataTable>(nullptr, *DataTablePath);
// 		if (!DataTable)
// 		{
// 			COS_LOG_ERROR(TEXT("DataTable '%s'을(를) 로드하지 못했거나 유효하지 않습니다."), *DataTablePath);
// 			return false;
// 		}
//
// 		const FAssetReferenceTableRow* Row = DataTable->FindRow<FAssetReferenceTableRow>(RowName, TEXT("LookupAssetPath"));
// 		if (!Row)
// 		{
// 			COS_LOG_ERROR(TEXT("DataTable '%s'에서 '%s' 행을 찾지 못했습니다."), *DataTablePath, *RowName.ToString());
// 			return false;
// 		}
//
// 		OutAssetPath = Row->AssetReference.ToString();
// 		return true;
// 	}
//
// 	// 자산 경로에서 T 유형의 자산을 로드합니다.
// 	template <typename T>
// 	static T* LoadAsset(const FString& AssetPath)
// 	{
// 		static_assert(TIsDerivedFrom<T, UObject>::Value, "T는 UObject에서 파생되어야 합니다.");
//
// 		T* Asset = LoadObject<T>(nullptr, *AssetPath);
// 		if (!Asset)
// 		{
// 			COS_LOG_ERROR(TEXT("경로 '%s'에서 유형 '%s'의 객체를 로드하지 못했습니다."), *AssetPath, *T::StaticClass()->GetName());
// 		}
// 		return Asset;
// 	}
//
// 	// 자산 경로를 사용하여 T 유형의 클래스를 찾습니다.
// 	template <typename T>
// 	static TSubclassOf<T> FindClass(const FString& AssetPath)
// 	{
// 		static_assert(TIsDerivedFrom<T, UObject>::Value, "T는 UObject에서 파생되어야 합니다.");
//
// 		ConstructorHelpers::FClassFinder<T> ClassFinder(*AssetPath);
// 		if (!ClassFinder.Succeeded())
// 		{
// 			COS_LOG_ERROR(TEXT("경로 '%s'에서 유형 '%s'의 클래스를 찾지 못했습니다."), *AssetPath, *T::StaticClass()->GetName());
// 			return nullptr;
// 		}
//
// 		return ClassFinder.Class;
// 	}
//
// 	// 하드코딩된 경로를 사용하여 클래스를 찾는 함수 추가
// 	template <typename T>
// 	static TSubclassOf<T> FindClassHardcoded(const FString& AssetPath)
// 	{
// 		static_assert(TIsDerivedFrom<T, UObject>::Value, "T는 UObject에서 파생되어야 합니다.");
//
// 		ConstructorHelpers::FClassFinder<T> ClassFinder(*AssetPath);
// 		if (!ClassFinder.Succeeded())
// 		{
// 			COS_LOG_ERROR(TEXT("경로 '%s'에서 유형 '%s'의 클래스를 찾지 못했습니다."), *AssetPath, *T::StaticClass()->GetName());
// 			return nullptr;
// 		}
//
// 		return ClassFinder.Class;
// 	}
// };
