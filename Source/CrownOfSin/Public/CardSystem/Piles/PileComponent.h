#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Interface_Pile.h"
#include "PileComponent.generated.h"


class ACardBase;


/*데크와 카드 패를 포함한 모든 카드 더미의 상위 클래스입니다.
 *더미에 있는 모든 카드에 대한 참조와 더미에서 카드를 추가하고 제거하는 방법을 보유합니다.*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UPileComponent : public UActorComponent, public IInterface_Pile
{
	GENERATED_BODY()

public:
	UPileComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable,Category="Pile Component")
	void ResolveAddNewCardBeyondMax();

	UFUNCTION(BlueprintCallable,Category="Pile Component")
	void RemoveAllCardsOwnedByActor(AActor* InActor);

	UFUNCTION(BlueprintCallable,Category="Pile Component")
	int32 GetCardCount();

protected:
	bool TryGetCardWithTags(const FGameplayTagContainer& RequiredTags, ACardBase*& OutCard);
	
	bool GetRandomCard(ACardBase*& OutCard);


	/*========================================================================================
	*	IInterface_Pile
	=========================================================================================*/
public:
	virtual void AssignExistingCards_Implementation(const TArray<ACardBase*>& InDeck) override;

	virtual bool ClearPile_Implementation() override;

	/**
     * 덱에서 카드를 요청하는 함수입니다.
     * 
     * @param RequestingPile    카드를 요청하는 다른 덱의 유형입니다.
     * @param SpecificCard      특정 카드를 요청할 경우, 해당 카드를 가리킵니다. nullptr일 경우 다른 조건에 따라 카드를 반환합니다.
     * @param bIsRandomCard     무작위 카드를 요청할 때 true로 설정합니다.
     * @param RequiredTags      필수로 요구되는 태그가 포함된 카드만 반환합니다.
     * @param OutCard           요청된 카드가 반환될 변수입니다. 찾지 못하면 nullptr을 반환합니다.
     * 
     * @return                  요청한 조건에 맞는 카드를 찾으면 true를 반환하고, 찾지 못하면 false를 반환합니다.
     */
	virtual bool RequestCardFromPile_Implementation(TSubclassOf<UPileComponent> RequestingPile, ACardBase* SpecificCard, bool bIsRandomCard, const FGameplayTagContainer& RequiredTags, ACardBase*& OutCard) override;

	virtual bool RequestRemoveCardFromPile_Implementation(ACardBase* InCard) override;
	
	virtual bool AttemptAddExistingCard_Implementation(ACardBase* InCard) override;

	virtual bool SpawnAndAddNewCard_Implementation(const FDataTableRowHandle& DataRowHandle, ACardBase*& OutCard) override;
	

	virtual TArray<ACardBase*> GetCards_Implementation() override;

	
	
public:
	/*========================================================================================
	*	FiledMembers
	=========================================================================================*/
public:
	UPROPERTY(BlueprintReadWrite, Category="Pile Component")
	FText FriendlyName;

	UPROPERTY(BlueprintReadWrite, Category="Pile Component")
	int32 MaxPileSize = 999;

	UPROPERTY(BlueprintReadWrite, Category="Pile Component")
	FGameplayTag PileTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pile Component", meta = (ExposeOnSpawn="true", RowType="/Script/CrownOfSin.ToolTip"))
	FDataTableRowHandle Tooltip;

	UPROPERTY(BlueprintReadWrite, Category="Pile Component")
	TArray<ACardBase*> Cards;
};
