#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "UW_HandSelect.generated.h"

class UUW_Card;
class ACardBase;
class UUW_CardListCard;
class UTextBlock;
class UOverlay;
class UHorizontalBox;
class UUW_CardHand;
class UButton;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmCards, const TArray<ACardBase*>&, InCards);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReturnCard, UWidget*, PairedCardWidget);


/**
 * UUW_HandSelect 클래스는 카드 선택 UI를 관리하는 위젯입니다.
 * 
 * 주요 기능:
 * - 플레이어가 지정된 수만큼 카드를 선택할 수 있도록 하는 기능
 * - 선택된 카드를 확인하거나 반환하는 이벤트 처리
 * - 선택된 카드와 관련된 위젯을 업데이트하고, 선택 상태를 표시
 *
 * 멤버 변수:
 * - 선택 가능한 카드 목록을 표시하는 박스, 명령 텍스트, 확인 버튼 등을 관리
 * - 선택 가능한 카드 태그와 관련된 정보를 저장
 * - 카드를 선택하거나 반환하는 이벤트 델리게이트를 제공
 */
UCLASS()
class CROWNOFSIN_API UUW_HandSelect : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_HandSelect(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

public:
	/**
	 * 카드 선택 이벤트를 초기화하는 함수입니다.
	 * - 플레이어가 선택할 카드 수, 선택할 수 있는 카드의 유효 태그를 설정하고 UI를 초기화합니다.
	 *
	 * @param CardCount 선택할 카드의 수
	 * @param InSourceHand 카드가 선택될 소스 핸드 위젯
	 * @param InValidCardTags 선택 가능한 카드의 유효 태그들
	 */
	UFUNCTION(BlueprintCallable, Category = "UW HandSelect")
	void InitializeEvent(int32 CardCount, UUW_CardHand* InSourceHand, const FGameplayTagContainer& InValidCardTags);

	/**
	 * 카드 선택 슬롯에 카드를 추가하려고 시도하는 함수입니다.
	 * - 선택 가능한 유효한 카드를 슬롯에 배치하고, 슬롯이 모두 차면 확인 버튼을 활성화합니다.
	 *
	 * @param CardActor 추가할 카드 액터
	 * @param PairedCardWidgetIn 추가할 카드와 연결된 위젯
	 * @return 카드가 성공적으로 추가되었는지 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "UW HandSelect")
	bool AttemptAddCard(ACardBase* CardActor, UUW_Card* PairedCardWidgetIn);

	/**
	 * 카드 선택 슬롯이 모두 찼는지 확인하고, 확인 버튼을 활성화합니다.
	 * - 모든 카드 슬롯이 차면 확인 버튼을 활성화하고, 그 상태를 반환합니다.
	 *
	 * @return 선택 슬롯이 모두 찼는지 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "UW HandSelect")
	bool EnableConfirmIfFull();

	/**
	 * 현재 선택된 카드들을 ContainedCards 배열에 추가하고, 모든 슬롯이 채워졌는지 확인합니다.
	 * - 선택된 카드를 LocalCards에 수집하고, 이를 전달받은 배열에 복사합니다.
	 * - 카드 슬롯이 모두 차 있는지 여부를 반환합니다.
	 *
	 * @param ContainedCards 선택된 카드가 담길 배열 (참조로 전달)
	 * @return 모든 카드 슬롯이 채워졌는지 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "UW HandSelect")
	bool GetContainedCards(TArray<ACardBase*>& ContainedCards);

protected:
	/**
	 * 확인 버튼이 클릭되었을 때 호출되는 함수입니다.
	 * - 현재 선택된 카드들을 가져와 이벤트를 브로드캐스트하고, 선택된 카드를 비활성화합니다.
	 */
	UFUNCTION()
	void ConfirmButtonOnClicked();

	/**
	 * 카드가 선택 슬롯에서 반환될 때 호출되는 함수입니다.
	 * - 선택된 카드를 선택 슬롯에서 제거하고, 카드 반환 이벤트를 브로드캐스트합니다.
	 *
	 * @param CardListCard 반환할 카드 슬롯
	 * @param CardActor 반환할 카드 액터
	 */
	UFUNCTION()
	void ReturnCardIfValid(UUW_CardListCard* CardListCard, ACardBase* CardActor);


	/*========================================================================================
	 *	Field Members
	 =========================================================================================*/
public:
	/** 선택된 카드를 표시하는 가로 박스 (카드 목록 UI) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW HandSelect|Designer", meta = (BindWidget))
	UHorizontalBox* CardSelectionBox;

	/** 플레이어에게 명령을 표시하는 텍스트 블록 (예: "카드를 선택하세요") */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW HandSelect|Designer", meta = (BindWidget))
	UTextBlock* CommandText;

	/** 카드 선택을 완료하는 버튼 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW HandSelect|Designer", meta = (BindWidget))
	UButton* ConfirmButton;

	/** 턴 종료를 표시하는 오버레이 (선택 완료 시 UI 변화) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW HandSelect|Designer", meta = (BindWidget))
	UOverlay* EndTurnOverlay;

	/** 카드 선택 UI에서 사용하는 이미지 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW HandSelect|Designer", meta = (BindWidget))
	UImage* Image_81;

public:
	/** 카드 선택에 사용되는 카드 목록 위젯 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW HandSelect|Config|Setup")
	TSubclassOf<UUW_CardListCard> WBP_CardListCardClass;

	/** 카드를 선택할 때 참조하는 카드 핸드 위젯 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW HandSelect")
	TObjectPtr<UUW_CardHand> SourceHand;

	/** 선택 가능한 카드에 대한 유효 태그 목록 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW HandSelect")
	FGameplayTagContainer ValidCardTags;

public:
	/** 카드를 선택 완료했을 때 호출되는 델리게이트 */
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="UW handSelect|Delegate")
	FOnConfirmCards OnConfirmCards;

	/** 카드를 선택 해제했을 때 호출되는 델리게이트 */
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="UW handSelect|Delegate")
	FOnReturnCard OnReturnCard;
};