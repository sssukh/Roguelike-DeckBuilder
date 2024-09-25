#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Interface_EventHolder.h"
#include "Interfaces/Interface_Interrupt.h"
#include "CardTransferComponent.generated.h"


class ACardBase;


/**
 * @brief UCardTransferComponent 클래스는 카드의 이동(전송)과 관련된 다양한 기능을 제공하는 컴포넌트입니다.
 * 
 * 이 컴포넌트는 카드가 더미 간 이동하거나, 핸드로 돌아가거나, 파괴, 제거되는 등의 동작을 관리합니다.
 * 
 * 주요 기능:
 * - 카드가 특정 더미(Discard, Destroy, Hand 등) 간 이동할 때 이벤트를 처리합니다.
 * - 카드를 새로 생성하거나 리셔플(Reshuffle)을 수행하는 기능을 포함하고 있습니다.
 * - 카드 이동 시 발생하는 중단(Interrupt) 기능을 지원합니다.
 * 
 * 카드 게임 내에서 카드의 전송 및 상태 변화와 관련된 대부분의 동작을 관리하는 데 사용됩니다.
 */
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardTransferComponent : public UActorComponent, public IInterface_Interrupt, public IInterface_EventHolder
{
	GENERATED_BODY()

public:
	UCardTransferComponent();

protected:
	virtual void BeginPlay() override;

public:
	/**
	 * @brief 카드를 리셔플하는 함수입니다. (카드가 덱으로 다시 섞임)
	 */
	UFUNCTION(Blueprintable, Category="Card Transfer Event")
	void Reshuffle();

	/**
	 * @brief 카드 관련 이벤트에 따라 적절한 더미(Pile)로 카드를 전송하는 함수입니다.
	 * 
	 * 이 함수는 다양한 카드 이벤트 (파괴, 버림, 드로우 등)에 대응하여 카드를 처리하며,특정 조건에 맞는 이벤트에 따라 카드를 적절한 더미로 전송하는 역할을 수행합니다.
	 * 또한 카드 생성, 핸드로 반환 등의 이벤트도 처리합니다.
	 *
	 * @param EventTag       발생한 카드 이벤트의 태그입니다.
	 * @param Card           처리할 카드 객체입니다.
	 * @param CallTags       이벤트와 관련된 추가 태그 컨테이너입니다.
	 */	
	UFUNCTION(Blueprintable, Category="Card Transfer Event")
	void RunCardTransferEvent(FGameplayTag EventTag, ACardBase* Card, FGameplayTagContainer CallTags);

	/**
	 * @brief 카드를 특정 소스 더미에서 타겟 더미로 전송하는 함수입니다.
	 * 
	 * 이 함수는 주어진 카드를 소스 더미에서 제거하고, 타겟 더미로 추가하는 작업을 처리합니다.
	 * 또한, 카드 이동과 관련된 애니메이션 및 액션을 실행합니다.
	 * 
	 * @param Card             전송할 카드 객체입니다.
	 * @param SourcePileTag    소스 더미를 나타내는 태그입니다.
	 * @param TargetPileTag    타겟 더미를 나타내는 태그입니다.
	 * @return 카드를 성공적으로 전송했으면 true, 실패했으면 false를 반환합니다.
	 */	
	UFUNCTION(Blueprintable, Category="Card Transfer Event")
	bool ResolveTransfer(ACardBase* Card, FGameplayTag SourcePileTag, FGameplayTag TargetPileTag);

	/**
	 * @brief 주어진 카드를 손 패(PileHand)로 되돌리는 함수입니다.
	 * 
	 * 이 함수는 특정 카드를 손 패에 추가하고, 그에 따라 "카드가 손으로 돌아갔다"는 이벤트를 전파합니다.
	 * 
	 * @param Card 전송할 카드 객체입니다.
	 * @param CallingObject 카드를 반환한 호출 객체입니다.
	 */	
	UFUNCTION(Blueprintable, Category="Card Transfer Event")
	void ReturnCardToHand(ACardBase* Card, UObject* CallingObject);

	/**
	 * @brief 주어진 카드를 생성하는 함수입니다. (화면에 표시되는 시간 포함)
	 * 
	 * @param Card 생성할 카드
	 * @param DisplayTime 화면에 표시될 시간
	 */
	UFUNCTION(Blueprintable, Category="Card Transfer Event")
	void GenerateCard(ACardBase* Card, float DisplayTime);

	/*========================================================================================
	*	IInterface_EventHolder
	=========================================================================================*/
public:
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;

	virtual float GetPriority_Implementation() override;

	/*========================================================================================
	*	IInterface_Interrupt
	=========================================================================================*/
public:
	virtual void CancelInterruption_Implementation() override;

	virtual void Interrupt_Implementation() override;

public:
	/** 카드 이동 중 인터럽트 여부를 나타냅니다. */
	UPROPERTY(BlueprintReadWrite, Category="Card Transfer")
	bool bInterrupt = false;

	/** 카드 이동 빈도를 나타냅니다. 기본값은 0.1초입니다. */
	UPROPERTY(BlueprintReadWrite, Category="Card Transfer")
	float CardTransferFrequency = 0.1f;

	/** 이벤트의 우선순위를 나타냅니다. */
	UPROPERTY(BlueprintReadWrite, Category="Card Transfer")
	float Priority=0;

	/** 카드가 이동할 현재 목표 더미의 태그입니다. */
	UPROPERTY(BlueprintReadWrite, Category="Card Transfer")
	FGameplayTag CurrentTargetPile;
};
