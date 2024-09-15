#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Utilities/CosLog.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/Object.h"
#include "UObject/SoftObjectPath.h"
#include "AssetTableRef.generated.h"


// 예를 들어, 어떤 자산을 로드하려면 다음과 같이 합니다.
/*
UTexture2D* MyTexture = FAssetReferenceUtility::LoadAssetFromDataTable(
	AssetRefPath::TexturesPath, FName("MyTextureRowName"));

// 클래스를 찾으려면 다음과 같이 합니다.
TSubclassOf<AActor> MyActorClass = FAssetReferenceUtility::FindClassFromDataTable<AActor>(
	AssetRefPath::BluePrintPath, FName("MyActorRowName"));
*/


// 자산 경로 상수 정의
namespace AssetRefPath
{
	const FString MeshesPath = TEXT("DataTable'/Game/CrownOfSin/AssetRef/DT_StaticMesh.DT_StaticMesh'");
	const FString MaterialPath = TEXT("DataTable'/Game/CrownOfSin/AssetRef/DT_Materials.DT_Materials'");
	const FString BluePrintPath = TEXT("DataTable'/Game/CrownOfSin/AssetRef/DT_BluePrints.DT_BluePrints'");
	const FString TexturesPath = TEXT("DataTable'/Game/CrownOfSin/AssetRef/DT_Textures.DT_Textures'");
	const FString DataTablePath = TEXT("DataTable'/Game/CrownOfSin/AssetRef/DT_DataTable.DT_DataTable'");
	// const FString WidgetPath = TEXT("DataTable'/Game/Game_/AssetRef/AssetRef_Widget.AssetRef_Widget'");
	// const FString SoundPath = TEXT("DataTable'/Game/Game_/AssetRef/AssetRef_Sounds.AssetRef_Sounds'");
	// const FString MaterialPath = TEXT("DataTable'/Game/Game_/AssetRef/DT_Materials.DT_Materials'");
}

USTRUCT(BlueprintType)
struct FAssetReferenceTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UObject> AssetReference;
};

/*DataTable 및 자산을 사용하는 유틸리티 기능을 제공합니다.*/
class FAssetReferenceUtility
{
public:
	// 데이터 테이블 로드 함수
	static UDataTable* LoadDataTable(const FName& RowName)
	{
		return LoadAssetFromDataTable<UDataTable>(AssetRefPath::DataTablePath, RowName);
	}

	// 텍스처 로드 함수
	static UTexture2D* LoadTexture(const FName& RowName)
	{
		return LoadAssetFromDataTable<UTexture2D>(AssetRefPath::TexturesPath, RowName);
	}

	// 머티리얼 로드 함수
	static UMaterialInterface* LoadMaterial(const FName& RowName)
	{
		return LoadAssetFromDataTable<UMaterialInterface>(AssetRefPath::MaterialPath, RowName);
	}

	// 메쉬 로드 함수
	static UStaticMesh* LoadMesh(const FName& RowName)
	{
		return LoadAssetFromDataTable<UStaticMesh>(AssetRefPath::MeshesPath, RowName);
	}

	// 블루프린트 클래스 로드 함수
	template <typename T>
	static TSubclassOf<T> LoadBlueprintClass(const FName& RowName)
	{
		return FindClassFromDataTable<T>(AssetRefPath::BluePrintPath, RowName, true);
	}

	
	
public:
	// 지정된 행 이름을 사용하여 지정된 DataTable에서 자산을 로드합니다.
	template <typename T>
	static T* LoadAssetFromDataTable(const FString& DataTablePath, const FName& RowName)
	{
		FString AssetPath;
		if (!IsInConstructorContext() || !TryGetAssetPathFromDataTable(DataTablePath, RowName, AssetPath))
		{
			return nullptr;
		}

		return LoadAsset<T>(AssetPath);
	}

	// DataTable의 행 이름을 사용하여 T 유형의 클래스를 찾습니다.
	template <typename T>
	static TSubclassOf<T> FindClassFromDataTable(const FString& DataTablePath, const FName& RowName, bool bIsBlueprint = false)
	{
		FString AssetPath;
		if (!IsInConstructorContext() || !TryGetAssetPathFromDataTable(DataTablePath, RowName, AssetPath))
		{
			return nullptr;
		}

		if (bIsBlueprint && !AssetPath.EndsWith("_C"))
		{
			AssetPath += TEXT("_C");
		}

		return FindClass<T>(AssetPath);
	}

private:
	// 현재 코드 실행이 생성자 컨텍스트 내에 있는지 확인합니다.
	static bool IsInConstructorContext()
	{
		const FUObjectThreadContext& ThreadContext = FUObjectThreadContext::Get();
		if (ThreadContext.IsInConstructor > 0)
		{
			return true;
		}

		COS_LOG_ERROR(TEXT("어설션 실패: 생성자 외부에서 호출되었습니다."));
		return false;
	}

	// DataTable에서 자산 경로를 가져옵니다.
	static bool TryGetAssetPathFromDataTable(const FString& DataTablePath, const FName& RowName, FString& OutAssetPath)
	{
		const UDataTable* DataTable = LoadObject<UDataTable>(nullptr, *DataTablePath);
		if (!DataTable || DataTable->GetRowStruct() == nullptr)
		{
			COS_LOG_ERROR(TEXT("DataTable '%s'을(를) 로드하지 못했거나 유효하지 않습니다."), *DataTablePath);
			return false;
		}

		const FAssetReferenceTableRow* Row = DataTable->FindRow<FAssetReferenceTableRow>(RowName, TEXT("LookupAssetPath"));
		if (!Row)
		{
			COS_LOG_ERROR(TEXT("DataTable '%s'에서 '%s' 행을 찾지 못했습니다."), *DataTablePath, *RowName.ToString());
			return false;
		}

		OutAssetPath = Row->AssetReference.ToString();
		return true;
	}

	// 자산 경로에서 T 유형의 자산을 로드합니다.
	template <typename T>
	static T* LoadAsset(const FString& AssetPath)
	{
		static_assert(TIsDerivedFrom<T, UObject>::Value, "T는 UObject에서 파생되어야 합니다.");

		T* Asset = LoadObject<T>(nullptr, *AssetPath);
		if (!Asset)
		{
			COS_LOG_ERROR(TEXT("경로 '%s'에서 유형 '%s'의 객체를 로드하지 못했습니다."), *AssetPath, *T::StaticClass()->GetName());
		}
		return Asset;
	}

	// 자산 경로를 사용하여 T 유형의 클래스를 찾습니다.
	template <typename T>
	static TSubclassOf<T> FindClass(const FString& AssetPath)
	{
		static_assert(TIsDerivedFrom<T, UObject>::Value, "T must be derived from UObject");

		ConstructorHelpers::FClassFinder<T> ClassFinder(*AssetPath);
		if (!ClassFinder.Class)
		{
			COS_LOG_ERROR(TEXT("경로 '%s'에서 유형 '%s'의 클래스를 찾지 못했습니다."), *AssetPath, *T::StaticClass()->GetName());
			return nullptr;
		}

		return ClassFinder.Class;
	}
};
