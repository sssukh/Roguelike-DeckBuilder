#pragma once

#include "CoreMinimal.h"
#include "ActionBase.h"
#include "GameplayTagContainer.h"
#include "Action_CardTransfer.generated.h"

class ACardBase;

/**
 * AAction_CardTransfer 클래스는 카드를 한 더미에서 다른 더미로 이동시키는 액션을 구현합니다.
 * 이 액션은 카드가 어느 더미에서 어디로 이동하는지 추적하고, 관련 UI 요소를 업데이트합니다.
 * 예를 들어, 카드가 플레이어의 손에서 버려지거나, 소모되거나, 다른 상태로 변환될 때 사용됩니다.
 */
UCLASS()
class CROWNOFSIN_API AAction_CardTransfer : public AActionBase
{
	GENERATED_BODY()

public:
	AAction_CardTransfer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetUpAction_Implementation() override;

	virtual void PlayAction_Implementation() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Card Transfer", meta=(ExposeOnSpawn="true"))
	ACardBase* Card;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Card Transfer", meta=(ExposeOnSpawn="true"))
	FGameplayTag SourcePile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Card Transfer", meta=(ExposeOnSpawn="true"))
	FGameplayTag TargetPile;

	UPROPERTY(BlueprintReadWrite, Category="Action Card Transfer")
	int32 SourcePileAmount;

	UPROPERTY(BlueprintReadWrite, Category="Action Card Transfer")
	int32 TargetPileAmount;
};
