#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_CardList.generated.h"


class UButton;
class UTextBlock;
class UUniformGridPanel;
class UUW_CardListCard;
class UPileComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPileRefSet_CardList, UPileComponent*, InPile);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOutSideClicked);

/**
 * UUW_CardList 클래스는 카드 리스트를 표시하고 관리하는 UI 위젯입니다.
 * 게임 내 덱이나 카드 풀의 상태를 시각적으로 표현하며, 카드를 GridPanel에 정렬하여 표시합니다.
 * 카드의 목록은 PileComponent에서 참조하며, 플레이어가 덱을 확인하거나 관리할 수 있도록 도와줍니다.
 * 
 * 주요 기능:
 * - 카드 목록을 업데이트하여 화면에 카드들을 표시
 * - 안쪽 버튼과 바깥쪽 버튼을 통해 UI 상호작용 처리
 * - 플레이어가 클릭한 요소에 따라 적절한 Delegate를 실행하여 이벤트를 처리
 * 
 * 이 위젯은 카드 시스템의 중요한 구성 요소로, 플레이어가 덱을 확인하거나 수정하는 과정에서 사용됩니다.
 */
UCLASS()
class CROWNOFSIN_API UUW_CardList : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_CardList(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

public:
	// 카드 리스트를 업데이트하는 함수
	UFUNCTION(BlueprintCallable, Category="UW Card List")
	void UpdateCardList();

protected:
	// 안쪽 버튼이 클릭되었을 때 호출되는 함수
	UFUNCTION()
	void OnClicked_InsideButton();

	// 바깥쪽 버튼이 클릭되었을 때 호출되는 함수
	UFUNCTION()
	void OnClicked_OutsideButton();


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(BlueprintReadWrite, Category="UW Card List|Designer", meta=(BindWidget))
	UUniformGridPanel* CardPanel;

	UPROPERTY(BlueprintReadWrite, Category="UW Card List|Designer", meta=(BindWidget))
	UTextBlock* PileName;

	UPROPERTY(BlueprintReadWrite, Category="UW Card List|Designer", meta=(BindWidget))
	UButton* InsideButton;

	UPROPERTY(BlueprintReadWrite, Category="UW Card List|Designer", meta=(BindWidget))
	UButton* OutsideButton;

public:
	// 카드 리스트에 표시될 개별 카드 위젯의 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UW Card List|Config|Setup")
	TSubclassOf<UUW_CardListCard> WBP_CardListCardClass;

	// 디버그 모드에서 카드 리스트가 표시될지 여부를 결정하는 플래그
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card List")
	bool bDebugDisplay = false;

	// 카드 리스트에서 한 행에 표시할 카드 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card List")
	int32 RowLength = 4;

	// 카드 리스트에서 참조할 Pile 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card List", meta=(ExposeOnSpawn="true"))
	UPileComponent* PileReference;

public:
	// Pile 컴포넌트가 설정되었을 때 호출되는 Delegate
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="UW Card List|Delegate")
	FOnPileRefSet_CardList OnPileRefSet;

	// 바깥쪽 버튼이 클릭되었을 때 호출되는 Delegate
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="UW Card List|Delegate")
	FOnOutSideClicked OnOutSideClicked;
};
