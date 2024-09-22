#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/CosEnumStruct.h"
#include "AttackPatternComponent.generated.h"


class ACardBase;
/* 미니언이 차례 동안 수행하는 작업의 선택 및 순서를 제어하는 데 사용됩니다.
기본 공격 패턴은 DT_AttackPattern에서 입력을 받아 미니언이 사용할 관련 카드를 생성하고 매 턴마다 카드를 선택할 확률을 처리합니다.
실제 표시 및 실행은 BP_Status_AttackPattern을 통해 처리됩니다.*/
UCLASS(ClassGroup=("COS|Combat"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UAttackPatternComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAttackPatternComponent();

protected:
	virtual void BeginPlay() override;

public:
	/**
	 * 공격 패턴에서 다음 카드를 선택합니다.
	 * 패턴이 끝에 도달하면 지정된 반복 지점으로 돌아가서 다시 카드를 선택합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="Attack Pattern Component")
	virtual ACardBase* GetNextCard();

	/**
	 * 주어진 카드 옵션 중에서 가중치 기반으로 무작위로 카드를 선택합니다.
	 * 선택된 카드는 반복 횟수와 조건에 따라 적절히 처리됩니다.
	 */
	UFUNCTION(BlueprintCallable, Category="Attack Pattern Component")
	ACardBase* ChooseRandomCardOption(const FCardOptions& InCardOptions);

	/**
	 * 주어진 카드가 이미 최대 반복 횟수에 도달했는지 확인하고, 해당 카드가 반복 가능하다면 반환합니다.
	 * 만약 반복 횟수가 초과되었을 경우, 다음 카드 옵션을 선택하여 반환합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="Attack Pattern Component")
	ACardBase* GetCardOrNextIfMaxRepeated(const FDataTableRowHandle& InCardData, int32 InMaxRepeat, int InCardOptionIndex, const FCardOptions& InCardOptions);

	/**
	 * 주어진 카드 데이터를 기반으로 고유한 카드 인스턴스를 생성하거나, 이미 생성된 카드가 있다면 이를 반환합니다.
	 * 카드가 이미 존재할 경우, 해당 카드의 상태를 초기화한 후 반환하며, 존재하지 않으면 새로운 카드를 생성합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="Attack Pattern Component")
	ACardBase* GetCardLazy(const FDataTableRowHandle& InCardData);

protected:
	/*소유자가 파괴되었을 때, 관리하고 있는 모든 카드 인스턴스를 정리하고 파괴합니다.*/
	UFUNCTION()
	void CleanUpOnOwnerDeath(AActor* DestroyedActor);

public:
	UPROPERTY(BlueprintReadWrite, Category="Attack Pattern")
	int32 PatternIndex = -1;

	UPROPERTY(BlueprintReadWrite, Category="Attack Pattern")
	int32 RepeatFrom = 0;

	UPROPERTY(BlueprintReadWrite, Category="Attack Pattern")
	int32 CurrentRepetitions = 0;

	UPROPERTY(BlueprintReadWrite, Category="Attack Pattern")
	ACardBase* PreviousPlayedCard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack Pattern", meta=(ExposeOnSpawn="true", RequiredAssetDataTags = "RowStructure=/Script/CrownOfSin.CardPattern"))
	FDataTableRowHandle AttackPatternData;

	UPROPERTY(BlueprintReadWrite, Category="Attack Pattern")
	TArray<FCardOptions> AttackPattern;

	UPROPERTY(BlueprintReadWrite, Category="Attack Pattern")
	TMap<FString, ACardBase*> Cards;
};
