#pragma once

#include "CoreMinimal.h"
#include "StatusComponent.h"
#include "Status_AttackPattern.generated.h"


class ACardBase;

UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_AttackPattern : public UStatusComponent
{
	GENERATED_BODY()

public:
	UStatus_AttackPattern();

protected:
	virtual void BeginPlay() override;

protected:
	/*
	 * 이벤트 발생 시 카드를 처리하고 상태를 업데이트하는 함수.
	 * 카드의 외형 변화, 상태값, 툴팁을 업데이트하고, 필요시 상태 수정 액션을 큐에 추가.
	 */
	void HandleRunEvent();

	// 카드의 외형(아이콘, 색상, 반복 횟수)이 변경되었는지 여부를 확인하고 카드의 아이콘, 색상 틴트, 반복 횟수를 비교하여 하나라도 달라졌다면 true를 반환.
	bool HasAppearanceChanged();

	 /*
	  *주어진 카드의 툴팁 정보를 업데이트하는 함수.
	  * 카드에서 툴팁 데이터를 가져와 로컬 배열에 저장한 후, 이를 멤버 변수 Tooltips에 적용.
	  */
	void UpdateTooltipsFromCard(ACardBase* InCard);
	

	/*========================================================================================
	*	IInterface_EventHolder
	=========================================================================================*/
public:
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(BlueprintReadWrite, Category="Status Attack Pattern")
	bool bAppearanceChanged = false;
	
	UPROPERTY(BlueprintReadWrite, Category="Status Attack Pattern")
	int32 LastCardRepetitions;
	
	UPROPERTY(BlueprintReadWrite, Category="Status Attack Pattern")
	int32 LastDisplayedStatusValue;
	
	UPROPERTY(BlueprintReadWrite, Category="Status Attack Pattern")
	float PreDisplayDelay = 0.3f;

	UPROPERTY(BlueprintReadWrite, Category="Status Attack Pattern")
	FGameplayTag CurrentEventTag;

	UPROPERTY(BlueprintReadWrite, Category="Status Attack Pattern")
	ACardBase* NextCard;

	UPROPERTY(BlueprintReadOnly, Category="Status Attack Pattern")
	TArray<AActor*> EmptyArray;

	UPROPERTY(BlueprintReadWrite, Category="Status Attack Pattern")
	UAttackPatternComponent* AttackPatternReference;
};
