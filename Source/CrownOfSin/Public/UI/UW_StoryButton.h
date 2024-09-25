#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardSystem/CardBase.h"
#include "Core/CosEnumStruct.h"
#include "UW_StoryButton.generated.h"


class UUW_ToolTip;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEmptyButtonClicked, UUserWidget*, InCallingWidget);

/**
 * UUW_StoryButton 클래스는 스토리 이벤트에서 선택지를 제공하는 버튼 위젯입니다.
 * 플레이어가 스토리 선택지를 클릭할 수 있으며, 클릭 시 관련된 카드 효과를 실행하거나
 * 버튼이 비활성화된 상태라면 관련 이벤트를 처리합니다.
 * 
 * 주요 기능:
 * - 버튼 클릭 시 관련된 카드 사용 또는 비어있는 선택지 처리
 * - 선택지가 유효한지 여부에 따라 버튼 활성화/비활성화
 * - 툴팁을 제공하여 선택지에 대한 추가 정보를 표시
 */
UCLASS()
class CROWNOFSIN_API UUW_StoryButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_StoryButton(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

public:
	/**
	 * 버튼의 사용 가능 여부를 갱신하는 함수입니다.
	 * 카드를 사용할 수 있는 상태인지 확인하고 버튼을 활성화 또는 비활성화합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Story Button Event")
	void RefreshPlayability();

protected:
	/**
	 * 스토리 버튼이 클릭되었을 때 호출되는 함수입니다.
	 * 카드가 유효하면 카드 효과를 실행하고, 유효하지 않으면 버튼 클릭 이벤트를 처리합니다.
	 */
	UFUNCTION()
	void OnClicked_StoryButton();

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(BlueprintReadWrite, Category="UW Story Button|Desinger", meta=(BindWidget))
	UButton* StoryButton;

public:
	/** 
	 * 툴팁 위젯의 클래스
	 * 선택지에 대한 추가 정보를 표시하는 툴팁 위젯입니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Story Button|Config|Setup")
	TSubclassOf<UUW_ToolTip> WBPToolTipClass;

	/** 
	 * 선택지에 해당하는 카드 데이터
	 * 플레이어가 선택한 스토리 옵션에 해당하는 카드 정보입니다.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="UW Story Button", meta=(ExposeOnSpawn="true"))
	FCard EncounterCard;

	/** 
	 * 생성된 카드 객체
	 * 선택지를 클릭하면 생성된 카드로 상호작용을 수행합니다.
	 */
	UPROPERTY(BlueprintReadWrite, Category="UW Story Button")
	ACardBase* SpawnedCard;

	/** 
	 * 버튼이 비어 있을 때 호출되는 델리게이트
	 * 유효한 카드가 없을 때 버튼이 클릭되면 이 델리게이트가 실행됩니다.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="UW Story Button|Delegate")
	FOnEmptyButtonClicked OnEmptyButtonClicked;
};
