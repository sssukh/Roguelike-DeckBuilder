#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Interface_EventHolder.h"
#include "UW_CardHand.generated.h"

class UImage;
class UCanvasPanel;
class UUW_CardVisual;
class UUW_TargetingBezier;
class ACardBase;
class UUW_Card;
class UUW_Anchor;
class UUW_HandSelect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReturnSelectedCards, const TArray<ACardBase*>&, Cards);

/**
 * UUW_CardHand 클래스는 게임 내에서 카드 핸들링과 관련된 다양한 기능을 담당하는 위젯 클래스입니다.
 * 
 * 주요 기능:
 * - 카드를 생성하고, 카드 위젯을 설정하여 화면에 표시하는 기능
 * - 카드 선택 모드, 카드 변환, 카드 이동 등을 처리하는 기능
 * - 글로벌 이벤트에 따른 카드 상태 업데이트
 * - 특정 카드 액션(생성, 사용, 이동 등)에 따라 카드 애니메이션을 제어하는 기능
 *
 * 멤버 변수:
 * - 손에 있는 카드 위젯, 앵커 위젯, 그리고 다양한 카드 상태를 관리하는 변수들을 포함
 * - 카드와 관련된 여러 상태 및 이벤트를 처리
 */
UCLASS()
class CROWNOFSIN_API UUW_CardHand : public UUserWidget, public IInterface_EventHolder
{
	GENERATED_BODY()

public:
	UUW_CardHand(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;


	/* ==========================================================================================
	 * 위젯 관리 (Widget Management)
	 * ========================================================================================== */
public:
	/**
	 * 앵커 위젯을 AnchorWidgets 맵에 추가하는 함수입니다.
	 * - 전달된 앵커 위젯들을 맵에 등록합니다.
	 *
	 * @param AnchorWidgetMap 등록할 앵커 위젯들이 담긴 맵 (FGameplayTag와 UUserWidget*의 매핑)
	 */
	UFUNCTION(BlueprintCallable, Category="UW Card Hand|Widget Management")
	void AddAnchorWidgets(const TMap<FGameplayTag, UUserWidget*>& AnchorWidgetMap);

	
	/* ==========================================================================================
	 * 카드 액션 (Card Actions)
	 * ========================================================================================== */
public:
	/**
	 * 카드 선택 모드를 활성화하는 함수입니다.
	 * - 주어진 카드 수와 유효한 카드 태그를 기반으로 선택 모드를 설정합니다.
	 *
	 * @param MaxSelectableCards 선택 가능한 최대 카드 수
	 * @param AllowedCardTags 선택 가능한 유효한 카드 태그들
	 */
	UFUNCTION(BlueprintCallable, Category="UW Card Hand|Card Actions")
	void EnableSelectionMode(int32 MaxSelectableCards, const FGameplayTagContainer& AllowedCardTags);

	/**
	 * 손에 있는 모든 카드를 숨기는 함수입니다.
	 * - 각 카드에 대해 숨기기 애니메이션을 실행합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Card Hand|Card Actions")
	void HideCardsInHand();

	/**
	 * 카드 액터를 기반으로 카드 위젯을 생성하고 설정하는 함수입니다.
	 * - 카드 액터를 받아 해당하는 위젯을 생성하고, 시작 변환을 설정한 후 카드 위젯을 손에 추가할지 여부를 결정합니다.
	 *
	 * @param CardActor 생성할 카드의 액터
	 * @param StartTransform 카드 위젯의 초기 변환값
	 * @param bAddToHand 위젯을 손에 추가할지 여부
	 * @return 생성된 카드 위젯
	 */
	UFUNCTION(BlueprintCallable, Category="UW Card Hand|Card Actions")
	UUW_Card* CreateAndSetupCardWidget(ACardBase* CardActor, const FWidgetTransform& StartTransform, bool bAddToHand);

	/**
	 * 카드를 다른 더미(소스)에서 목표 더미로 전송하는 함수입니다.
	 * - 소스와 목표 더미에 따라 카드를 전송하고, 필요시 카드 위젯을 생성하거나 애니메이션을 재생합니다.
	 *
	 * @param InCard 전송할 카드
	 * @param SourcePile 카드의 현재 위치(소스 더미)
	 * @param TargetPile 카드를 전송할 목표 더미
	 */
	UFUNCTION(BlueprintCallable, Category="UW Card Hand|Card Actions")
	void TransferCardAction(ACardBase* InCard, FGameplayTag SourcePile, FGameplayTag TargetPile);

	
	/* ==========================================================================================
	 * 좌표 및 위치 계산 (Position & Coordinate Calculation)
	 * ========================================================================================== */
public:
	/**
	 * 손 앵커의 현재 화면 위치를 반환하는 함수입니다.
	 * - 손의 위치를 화면 좌표계로 변환하여 반환합니다.
	 * @return 손 앵커의 뷰포트 좌표계 위치
	 */
	UFUNCTION(BlueprintPure, Category="UW Card Hand|Position Calculation")
	FVector2D GetHandPosition();

	/**
	 * 드로우 위치에 오프셋을 적용한 값을 반환하는 함수입니다.
	 * - 드로우 위치에 미리 설정된 오프셋을 더한 값을 반환합니다.
	 * @return 드로우 위치에 오프셋을 더한 위치값
	 */
	UFUNCTION(BlueprintPure, Category="UW Card Hand|Position Calculation")
	FVector2D GetOffsetDrawPosition();

	/**
	 * 지정된 앵커 위젯의 화면 좌표 위치를 반환하는 함수입니다.
	 * - 주어진 FGameplayTag에 해당하는 앵커 위젯의 뷰포트 위치를 계산하고, 오프셋을 적용한 값을 반환합니다.
	 * 
	 * @param AnchorWidget 화면 좌표를 가져올 앵커 위젯의 FGameplayTag
	 * @return 앵커 위젯의 뷰포트 좌표에 오프셋을 더한 위치
	 */
	UFUNCTION(BlueprintCallable, Category="UW Card Hand|Position Calculation")
	FVector2D GetAnchorWidgetPosition(FGameplayTag AnchorWidget);


	/* ==========================================================================================
	 * 카드 변환 및 업데이트 (Card Transformation & Updates)
	 * ========================================================================================== */
public:
	/**
	 * 주기적으로 카드의 변환을 업데이트하도록 타이머를 설정하는 함수입니다.
	 * - 첫 번째 호출 시 즉시 카드 변환을 업데이트한 후, 타이머를 설정하여 반복적으로 업데이트합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="UW card Hand|Card Transformation")
	void UpdatePeriodically();

	/**
	 * 손에 있는 카드 위젯들의 변환(위치, 크기, 회전 등)을 업데이트하는 함수입니다.
	 * - 각 카드의 위치와 크기를 계산하여 적절한 변환값을 설정합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="UW card Hand|Card Transformation")
	void UpdateCardTransforms();

	/**
	 * 카드 변환을 처리하는 함수입니다.
	 * - 주기적으로 호출되어 카드의 변환을 업데이트합니다.
	 */
	UFUNCTION()
	void HandleCardTransforms();


	/* ==========================================================================================
	 * 이벤트 처리 (Event Handling)
	 * ========================================================================================== */
protected:
	UFUNCTION()
	void BindHandToCardWidgetEvents(UUW_Card* InCardWidget);
	
	/**
	 * 카드 위젯이 제거될 때 해당 카드를 위젯 맵에서 삭제하는 함수입니다.
	 * - 카드가 맵에 있는지 확인한 후, 위젯이 일치하면 맵에서 제거합니다.
	 *
	 * @param RemovedCard 제거할 카드
	 * @param RemovedWidget 제거할 카드에 해당하는 위젯
	 */
	UFUNCTION()
	void RemoveCard(ACardBase* RemovedCard, UUserWidget* RemovedWidget);

	/**
	 * 카드 위젯에서 발생한 이벤트를 처리하는 함수입니다.
	 * - 카드 선택 모드일 때 카드를 추가하거나, 일반 모드에서 카드를 잡거나 놓는 이벤트를 처리합니다.
	 *
	 * @param InEvent 처리할 이벤트의 FGameplayTag
	 * @param InCardWidget 이벤트가 발생한 카드 위젯
	 */
	UFUNCTION()
	void ResolveCardWidgetEvent(FGameplayTag InEvent, UUW_Card* InCardWidget);

	/**
	 * 선택 모드에서 반환된 카드를 다시 보이게 하는 함수입니다.
	 * - 선택된 카드를 손으로 다시 반환할 때, 해당 카드 위젯의 가시성을 설정합니다.
	 * 
	 * @param PairedCardWidget 선택 모드에서 반환된 카드 위젯
	 */
	UFUNCTION()
	void ReturnCardFromHandSelect(UWidget* PairedCardWidget);

	/**
	 * 선택 모드에서 선택된 카드들을 확정하는 함수입니다.
	 * - 선택된 카드 목록을 브로드캐스트하고, 선택 모드를 종료합니다.
	 * 
	 * @param SelectedCards 선택된 카드들의 배열
	 */
	UFUNCTION()
	void ConfirmSelectedCards(const TArray<ACardBase*>& SelectedCards);

	/**
	 * 카드를 드로우하여 손에 추가하거나 애니메이션을 처리하는 함수입니다.
	 * @param StartAnchor 카드를 드로우할 앵커 태그
	 */
	void HandleTransferCardAction(FGameplayTag StartAnchor);

	
	/*========================================================================================
	*	IInterface_EventHolder
	=========================================================================================*/
public:
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;

	
	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	/** 카드 위젯을 표시할 캔버스 패널 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card Hand", meta=(BindWidget))
	UCanvasPanel* HandCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card Hand", meta=(BindWidget))
	UImage* CardZone;

	/** 카드가 손에 들어가는 앵커 위젯 (핸드 앵커) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card Hand", meta=(BindWidget))
	UUW_Anchor* WBP_HandAnchor;

	/** 카드가 플레이될 때 표시되는 앵커 위젯 (플레이 앵커) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card Hand", meta=(BindWidget))
	UUW_Anchor* WBP_PlayAnchor;

	/** 카드가 섞이는 때에 표시되는 앵커 위젯 (섞기 앵커) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card Hand", meta=(BindWidget))
	UUW_Anchor* WBP_ReshuffleAnchor;

	/** 카드 선택 모드를 위한 위젯 (카드 선택 핸들링) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card Hand", meta=(BindWidget))
	UUW_HandSelect* WBP_HandSelect;

	/** 타겟팅 베지어를 처리하는 위젯 (카드 이동 시각화) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card Hand", meta=(BindWidget))
	UUW_TargetingBezier* WBP_Bezier;

public:
	/** 생성할 카드 위젯의 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card Hand|Config|Setup")
	TSubclassOf<UUW_Card> WBP_CardClass;

	/** 카드 시각적 요소를 처리할 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card Hand|Config|Setup")
	TSubclassOf<UUW_CardVisual> WBP_CardVisualClass;

	/** 카드 핸들링이 활성화되었는지 여부를 나타내는 플래그 */
	UPROPERTY(BlueprintReadWrite, Category="UW Card Hand")
	bool bEnabled = true;

	/** 현재 손에 카드를 들고 있는지 여부를 나타내는 플래그 */
	UPROPERTY(BlueprintReadWrite, Category="UW Card Hand")
	bool bCardInHand = false;

	/** 카드 선택 모드가 활성화되었는지 여부를 나타내는 플래그 */
	UPROPERTY(BlueprintReadWrite, Category="UW Card Hand")
	bool bSelectionMode = false;

	/** 카드 변환을 주기적으로 업데이트하는 시간 간격 (초 단위) */
	UPROPERTY(BlueprintReadWrite, Category="UW Card Hand")
	float UpdatePeriod = 0.5f;

	/** 카드가 생성될 때 부여할 ZOrder 값 */
	UPROPERTY(BlueprintReadWrite, Category="UW Card Hand")
	int32 GenerateZOrder = 10000;

	/** 카드 간의 간격 (손에 있을 때 카드 간 거리) */
	UPROPERTY(BlueprintReadWrite, Category="UW Card Hand")
	float CardSpacing = 120.0f;

	/** 카드가 손에 있을 때 가운데로부터 회전할 각도 */
	UPROPERTY(BlueprintReadWrite, Category="UW Card Hand")
	float CardAngleFromCenter = 2.5f;

	/** 카드의 기본 스케일 (크기) */
	UPROPERTY(BlueprintReadWrite, Category="UW Card Hand")
	FVector2D BaseCardScale = FVector2D(0.6f, 0.6f);

	/** 드로우 위치 (카드가 처음 나타나는 위치) */
	UPROPERTY(BlueprintReadWrite, Category="UW Card Hand")
	FVector2D DrawPosition;

	/** 드로우 위치에 적용할 오프셋 (카드가 드로우될 때 위치 조정) */
	UPROPERTY(BlueprintReadWrite, Category="UW Card Hand")
	FVector2D DrawOffset = FVector2D(300.0f, 300.0f);

	/** 앵커에 적용할 위치 오프셋 (앵커 위치 조정) */
	UPROPERTY(BlueprintReadWrite, Category="UW Card Hand")
	FVector2D AnchorOffset = FVector2D(40.f, 40.0f);

	/** 손에 있는 카드의 크기 */
	UPROPERTY(BlueprintReadWrite, Category="UW Card Hand")
	FVector2D CardSizeInHand = FVector2D(384.f, 512.0f);

	/** 현재 처리 중인 이벤트 태그 */
	UPROPERTY(BlueprintReadWrite, Category="UW Card Hand")
	FGameplayTag CurrentEventTag;

	/** 현재 처리 중인 이벤트와 관련된 카드 액터 */
	UPROPERTY(BlueprintReadWrite, Category="UW Card Hand")
	ACardBase* CurrentEventCard;

	UPROPERTY(BlueprintReadWrite, Category="UW Card Hand")
	UUW_Card* HeldCard;

	/** 앵커 위젯들을 저장하는 맵 (GameplayTag와 UUserWidget의 매핑) */
	UPROPERTY(BlueprintReadWrite, Category="UW Card Hand")
	TMap<FGameplayTag, UUserWidget*> AnchorWidgets;

	/** 카드와 위젯을 매핑하는 맵 (ACardBase와 UUW_Card의 매핑) */
	UPROPERTY(BlueprintReadWrite, Category="UW Card Hand")
	TMap<ACardBase*, UUW_Card*> CardWidgets;

	/** 손에 있는 카드 위젯 배열 */
	UPROPERTY(BlueprintReadWrite, Category="UW Card Hand")
	TArray<UUW_Card*> CardWidgetsInHand;

private:
	/** 카드 업데이트를 위한 타이머 핸들 */
	UPROPERTY()
	FTimerHandle UpdateTimerHandle;

public:
	/** 선택된 카드 목록을 반환하는 델리게이트 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="UW Card Hand|Delegate")
	FReturnSelectedCards OnReturnSelectedCards;
};