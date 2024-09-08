#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "CardSystem/CardPlayer.h"
#include "CardSystem/Piles/PileComponent.h"
#include "CardEffect_AddRandomCardsToPile.generated.h"

/*대상 구성 요소에 지정된 더미에 희귀도의 효과 값과 동일한 수의 무작위 카드를 추가합니다.*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_AddRandomCardsToPile : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	UCardEffect_AddRandomCardsToPile();

protected:
	virtual void BeginPlay() override;

public:
	virtual bool ResolveCardEffect(AActor* TargetActor) override;

protected:
	// 타겟이 Pile 컴포넌트의 자식이면 해당 클래스 반환, 아니면 Hand Pile 컴포넌트 반환
	TSubclassOf<UPileComponent> GetTargetPileClass(UClass* InTargetComponent) const;

	// 랜덤 카드를 생성하고 Pile에 추가하는 함수
	bool AddRandomCardsToPile(ACardPlayer* CardPlayer, TSubclassOf<UPileComponent> TargetPileClass);

	// 새로운 카드를 스폰하는 함수
	ACardBase* SpawnNewCard(const FCard& RandomCard);


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	//아직 쓰이는 곳이 없습니다. 생성할 때 미리 값이 생성되어 있습니다.
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Card Effect Add Random Cards To Pile", meta = (RowType="/Script/CrownOfSin.Card"))
	// FDataTableRowHandle DataRowHandle;
};
