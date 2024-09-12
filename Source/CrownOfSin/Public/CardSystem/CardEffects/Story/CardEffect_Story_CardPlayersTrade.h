#pragma once

#include "CoreMinimal.h"
#include "CardSystem/CardEffects/CardEffectComponent.h"
#include "CardEffect_Story_CardPlayersTrade.generated.h"

/*플레이어가 선택한 대상 카드의 희귀도에 따라 계속할 만남을 결정하기 위해 "카드 플레이어" 스토리 만남에서 사용됩니다.*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_Story_CardPlayersTrade : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	UCardEffect_Story_CardPlayersTrade();

protected:
	virtual void BeginPlay() override;

public:
	virtual bool ResolveCardEffect(AActor* TargetActor) override;

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="CardEffect Story CardPlayers Trade")
	FDataTableRowHandle CommonEncounter;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="CardEffect Story CardPlayers Trade")
	FDataTableRowHandle EpicEncounter;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="CardEffect Story CardPlayers Trade")
	FDataTableRowHandle TrashEncounter;
};
