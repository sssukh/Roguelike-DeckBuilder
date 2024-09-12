#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Interface_EventHolder.h"
#include "Interfaces/Interface_Pile.h"
#include "PileComponent.generated.h"


class ACardBase;


/**
 * UPileComponent 클래스는 카드 게임에서 카드 더미(Pile)를 관리하는 역할을 담당하는 컴포넌트입니다.
 * 이 클래스는 카드 더미에 카드를 추가하거나 제거하고, 요청된 카드를 반환하는 등의 다양한 기능을 제공합니다.
 * 또한, 더미의 최대 크기 제한을 관리하며, 태그나 무작위 방식으로 카드를 검색할 수 있는 기능도 포함하고 있습니다.
 * 이 컴포넌트는 게임 내에서 여러 카드 관련 기능을 수행하는 데 사용되며, 다른 더미 컴포넌트와 상호작용할 수 있습니다.
 */
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UPileComponent : public UActorComponent, public IInterface_Pile,public IInterface_EventHolder
{
	GENERATED_BODY()

public:
	UPileComponent();

protected:
	virtual void BeginPlay() override;

public:
	/**
 	 * 더미의 최대 크기(MaxPileSize)를 초과하여 카드를 추가하려고 할 때 호출됩니다.
 	 * 이 함수는 추가 시도를 기록하고 경고 메시지를 출력합니다.
 	 */
	UFUNCTION(BlueprintCallable, Category="Pile Component Event")
	void ResolveAddNewCardBeyondMax();

	/**
 	 * 주어진 액터가 소유한 모든 카드를 제거합니다.
 	 * 더미에 있는 카드 중 소유자가 호출한 액터와 동일한 경우, 해당 카드를 "무효화" 처리합니다.
 	 * @param InActor 카드를 소유한 액터입니다.
 	 */
	UFUNCTION(BlueprintCallable, Category="Pile Component Event")
	void RemoveAllCardsOwnedByActor(AActor* InActor);

	/**
 	 * 현재 더미에 포함된 카드의 개수를 반환합니다.
 	 * @return 더미에 있는 카드의 총 개수입니다.
 	 */
	UFUNCTION(BlueprintPure, Category="Pile Component Event")
	int32 GetCardCount();

protected:
	/**
 	 * 지정된 태그를 모두 가진 카드를 찾아 반환하는 함수입니다. 
 	 * @param RequiredTags     검색할 태그들이 포함된 태그 컨테이너입니다.
 	 * @param OutCard          검색 결과로 반환할 카드입니다. 태그 조건에 맞는 카드를 찾지 못하면 nullptr을 반환합니다.
 	 * @return 태그 조건에 맞는 카드를 찾으면 true를 반환하고, 찾지 못하면 false를 반환합니다.
 	 */
	bool TryGetCardWithTags(const FGameplayTagContainer& RequiredTags, ACardBase*& OutCard);

	/**
 	 * 카드 더미에서 무작위로 카드를 선택하여 반환하는 함수입니다. 
 	 * @param OutCard          무작위로 선택된 카드가 저장될 출력 변수입니다. 카드를 찾지 못하면 nullptr을 반환합니다.
 	 * @return 카드를 성공적으로 선택했으면 true, 더미에 카드가 없거나 실패한 경우 false를 반환합니다.
 	 */
	bool GetRandomCard(ACardBase*& OutCard);


	/*========================================================================================
	*	IInterface_Pile
	=========================================================================================*/
public:
	/**
 	 * 전달된 카드 배열을 순회하면서 더미에 각 카드를 추가하는 함수입니다. 
 	 * @param InDeck   더미에 추가할 기존 카드 배열입니다.
 	 */
	virtual void AssignExistingCards_Implementation(const TArray<ACardBase*>& InDeck) override;

	/**
 	 * 더미에 있는 모든 카드를 제거하고, 제거된 카드를 처리하는 컴포넌트로 전달하는 함수입니다.
 	 * @return 카드를 성공적으로 제거했으면 true, 실패했으면 false를 반환합니다.
 	 */
	virtual bool ClearPile_Implementation() override;

	/**
	  * 다른 더미에서 카드를 요청할 때 사용되는 함수입니다.
	  * 특정 카드를 요청하거나, 조건에 맞는 무작위 또는 첫 번째 카드를 반환할 수 있습니다.
	  * 
	  * @param RequestingPile   카드를 요청하는 더미의 클래스입니다.
	  * @param SpecificCard     특정 카드를 요청할 때 사용됩니다. nullptr인 경우 다른 조건에 따라 카드를 반환합니다.
	  * @param bIsRandomCard    무작위 카드를 요청할 때 true로 설정됩니다.
	  * @param RequiredTags     필수 태그가 있을 경우 해당 태그를 가진 카드만 반환합니다.
	  * @param OutCard          요청된 카드가 저장될 출력 변수입니다. 찾지 못하면 nullptr을 반환합니다.
	  * 
	  * @return 요청된 조건에 맞는 카드를 찾으면 true를 반환하고, 찾지 못하면 false를 반환합니다.
	  */
	virtual bool RequestCardFromPile_Implementation(TSubclassOf<UPileComponent> RequestingPile, ACardBase* SpecificCard, bool bIsRandomCard, const FGameplayTagContainer& RequiredTags,
	                                                ACardBase*& OutCard) override;

	/**
 	 * 더미에서 요청된 카드를 제거하는 함수입니다.
 	 * 
 	 * @param InCard   더미에서 제거할 카드입니다.
 	 * 
 	 * @return 카드를 성공적으로 제거했으면 true, 제거하지 못했으면 false를 반환합니다.
 	 */
	virtual bool RequestRemoveCardFromPile_Implementation(ACardBase* InCard) override;

	/**
 	 * 기존 카드를 더미에 추가하는 함수입니다.
 	 * 
 	 * @param InCard   더미에 추가할 카드입니다.
 	 * 
 	 * @return 더미에 카드를 성공적으로 추가하면 true, 더미의 최대 크기를 초과한 경우 false를 반환합니다.
 	 */
	virtual bool AttemptAddExistingCard_Implementation(ACardBase* InCard) override;

	/**
 	 * 데이터 테이블에서 카드를 생성하여 더미에 추가하는 함수입니다.
 	 * 
 	 * @param DataRowHandle    생성할 카드의 데이터 테이블 행을 나타내는 핸들입니다.
 	 * @param OutCard          생성된 카드가 저장될 출력 변수입니다.
 	 * 
 	 * @return 카드를 성공적으로 생성하고 더미에 추가하면 true, 더미의 최대 크기를 초과하거나 실패한 경우 false를 반환합니다.
 	 */
	virtual bool SpawnAndAddNewCard_Implementation(const FDataTableRowHandle& DataRowHandle, ACardBase*& OutCard) override;

	/**
 	 * 현재 더미에 있는 모든 카드를 반환하는 함수입니다.
 	 * 
 	 * @return 더미에 저장된 카드 배열을 반환합니다.
 	 */
	virtual TArray<ACardBase*> GetCards_Implementation() override;

public:
	/*========================================================================================
	*	FiledMembers
	=========================================================================================*/
public:
	/** 더미의 이름 또는 별칭을 나타냅니다. */
	UPROPERTY(BlueprintReadWrite, Category="Pile Component")
	FText FriendlyName;

	/** 더미에 포함될 수 있는 최대 카드 수입니다. 기본값은 999입니다. */
	UPROPERTY(BlueprintReadWrite, Category="Pile Component")
	int32 MaxPileSize = 999;

	/** 더미에 할당된 태그로, 더미를 식별하는 데 사용됩니다. */
	UPROPERTY(BlueprintReadWrite, Category="Pile Component")
	FGameplayTag PileTag;

	/** 더미와 관련된 툴팁 데이터를 나타내는 핸들입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pile Component", meta = (ExposeOnSpawn="true", RowType="/Script/CrownOfSin.ToolTip"))
	FDataTableRowHandle Tooltip;

	/** 더미에 포함된 카드들의 배열입니다. */
	UPROPERTY(BlueprintReadWrite, Category="Pile Component")
	TArray<ACardBase*> Cards;
};
