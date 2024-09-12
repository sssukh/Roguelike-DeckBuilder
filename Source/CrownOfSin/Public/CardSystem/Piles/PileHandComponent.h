#pragma once

#include "CoreMinimal.h"
#include "PileComponent.h"
#include "PileHandComponent.generated.h"

/**
 * UPileHandComponent 클래스는 UPileComponent를 상속받아 카드 게임에서 플레이어가 손에 들고 있는 카드들을 관리하는 역할을 담당하는 컴포넌트입니다.
 * 
 * 이 클래스는 카드 더미에서 카드를 뽑고, 손에 있는 카드를 버리거나 수정하는 등의 기능을 제공합니다.
 * 또한, 다른 카드 더미로부터 요청한 카드를 뽑아오는 기능을 포함하고 있으며, 카드가 손에 들어오거나 나갈 때 발생하는 이벤트를 처리합니다.
 * 
 * 주요 기능:
 * - 지정된 카드 더미로부터 카드를 뽑는 기능을 제공합니다.
 * - 손에 있는 모든 카드를 버릴 수 있는 기능을 제공합니다.
 * - 손에 있는 카드의 순서를 무작위로 뽑을 수 있는 옵션을 제공합니다.
 * - 손에 들어온 카드와 나가는 카드에 대해 이벤트 처리를 수행합니다.
 * 
 * 이 클래스는 카드 게임에서 플레이어의 손 패에 대한 로직을 처리하는 데 사용됩니다.
 */
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UPileHandComponent : public UPileComponent
{
	GENERATED_BODY()

public:
	UPileHandComponent();

protected:
	virtual void BeginPlay() override;

public:
	/**
 	 * AttemptDraw 함수는 현재 손에 든 카드의 수가 최대 허용 수(MaxPileSize)보다 적을 경우,
 	 * 카드 더미에서 새로운 카드를 뽑는 기능을 수행합니다.
 	 * 만약 카드를 성공적으로 뽑으면 DrawCard 함수가 호출되어 손에 추가됩니다.
 	 */
	UFUNCTION(BlueprintCallable, Category="Pile Hand Component Event")
	void AttemptDraw();

	/**
 	 * DrawCard 함수는 지정된 카드 또는 무작위 카드를 주어진 출처의 카드 더미에서 뽑아 손에 추가하는 기능을 수행합니다.
 	 * 만약 지정된 카드가 없으면 무작위로 카드를 선택할 수 있으며, 카드가 없을 경우 리셔플(Reshuffle)을 시도합니다.
 	 * @param SpecifiedCard - 요청된 특정 카드 (nullptr일 경우 무작위 카드를 뽑음).
 	 * @param SourcePile - 카드를 뽑을 출처가 되는 더미의 클래스.
 	 * @param OutDrawnCard - 뽑힌 카드를 저장할 출력 변수.
 	 * @return 카드를 성공적으로 뽑으면 true, 실패하면 false를 반환합니다.
 	 */
	UFUNCTION(BlueprintCallable, Category="Pile Hand Component Event")
	bool DrawCard(ACardBase* SpecifiedCard, TSubclassOf<UPileHandComponent> SourcePile, ACardBase*& OutDrawnCard);

	/**
 	 * DiscardHand 함수는 현재 손에 들고 있는 모든 카드를 처리하여 버리는 기능을 수행합니다.
 	 * 각각의 카드는 턴 종료 시점의 패시브 이벤트(Flag_Passive)를 발생시켜 필요한 처리를 진행합니다.
 	 */
	UFUNCTION(BlueprintCallable, Category="Pile Hand Component Event")
	void DiscardHand();

	/*========================================================================================
	*	IInterface_EventHolder
	=========================================================================================*/
public:
	/**
 	 * RunEvent_Implementation 함수는 지정된 이벤트 태그에 따라 특정 이벤트를 처리하는 역할을 합니다.
 	 * 주로 카드가 손에 있을 때 발생하는 수정 이벤트(Event_Card_ModifyInHand)를 처리하며, 카드를 호출한 객체가 손에 있는 경우 그 카드의 수정 상태를 초기화합니다.
 	 * 
 	 * @param EventTag - 발생한 이벤트의 태그.
 	 * @param CallingObject - 이벤트를 호출한 객체.
 	 * @param bIsGlobal - 이벤트가 전역 범위에서 발생했는지 여부.
 	 * @param PayLoad - 추가적인 정보가 담긴 객체.
 	 * @param CallTags - 이벤트에 포함된 추가 태그.
 	 */
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;


	/*========================================================================================
	*	IInterface_Pile
	=========================================================================================*/
public:
	/**
 	 * AttemptAddExistingCard_Implementation 함수는 주어진 카드를 손 패에 추가하려고 시도하는 함수입니다.
 	 * 기본 PileComponent의 카드 추가 기능을 사용하며, 성공적으로 추가된 경우 카드를 손에 넣었다는 이벤트를 발생시킵니다.
 	 * 
 	 * @param InCard - 추가하려는 카드 객체.
 	 * @return 카드를 성공적으로 추가했으면 true, 실패했으면 false를 반환합니다.
 	 */
	virtual bool AttemptAddExistingCard_Implementation(ACardBase* InCard) override;

	/**
 	 * RequestRemoveCardFromPile_Implementation 함수는 주어진 카드를 손 패에서 제거하는 기능을 수행합니다.
 	 * 기본 PileComponent의 카드 제거 기능을 사용하며, 카드를 성공적으로 제거한 후 손 패에서 나갔다는 이벤트를 발생시킵니다.
 	 * 또한, 손 패에서의 카드 수정 상태를 초기화합니다.
 	 * 
 	 * @param InCard - 제거하려는 카드 객체.
 	 * @return 카드를 성공적으로 제거했으면 true를 반환합니다.
 	 */
	virtual bool RequestRemoveCardFromPile_Implementation(ACardBase* InCard) override;


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	/* 손 패에서 마지막으로 뽑은 카드를 저장하는 변수입니다.
	 * 이 변수는 카드를 뽑는 메서드에서 사용되며, 뽑힌 카드가 할당됩니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pile Hand Component", meta=(ExposeOnSpawn="true"))
	ACardBase* DrawnCard;

	/* 카드를 무작위로 뽑을지 여부를 결정하는 변수입니다.
	 * true로 설정된 경우, 카드 더미에서 무작위로 카드를 뽑습니다.
	 * false일 경우 순서대로 카드를 뽑습니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pile Hand Component", meta=(ExposeOnSpawn="true"))
	bool bRandomDrawOrder = true;
};
