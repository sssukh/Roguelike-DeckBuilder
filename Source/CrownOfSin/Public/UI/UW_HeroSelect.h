#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/CosEnumStruct.h"
#include "UW_HeroSelect.generated.h"

class UComboBoxString;
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_HeroSelect : public UUserWidget
{
	GENERATED_BODY()

public:
	// 선택된 영웅과 덱에 대한 데이터를 가져와 FHeroDeck 구조체에 저장합니다.
	// HeroTable 및 DeckTable 데이터 테이블에서 선택된 옵션에 따라 해당 영웅과 덱의 행 이름을 설정합니다.
	// 이 함수는 UI 선택기를 통해 선택된 영웅과 덱의 데이터를 반환하는 역할을 합니다.
	void GetSelectedData(FHeroDeck& OutHeroDeck);


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UW Hero Select|Designer", meta=(BindWidget))
	UComboBoxString* HeroSelector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UW Hero Select|Designer", meta=(BindWidget))
	UComboBoxString* DeckSelector;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UW Hero Select", meta = (ExposeOnSpawn="true", RequiredAssetDataTags = "RowStructure=/Script/CrownOfSin.Minion"))
	UDataTable* HeroTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UW Hero Select", meta = (ExposeOnSpawn="true", RequiredAssetDataTags = "RowStructure=/Script/CrownOfSin.Deck"))
	UDataTable* DeckTable;
};
