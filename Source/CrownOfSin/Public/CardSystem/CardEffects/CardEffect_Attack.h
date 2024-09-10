
#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "CardEffect_Attack.generated.h"

/*대상의 방어구에서 효과 값을 빼고 나머지는 체력에서 뺍니다. 입힌 피해에 따라 화면 흔들기 동작을 호출합니다.*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_Attack : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCardEffect_Attack();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual bool ResolveCardEffect(AActor* TargetActor) override;

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(BlueprintReadWrite,Category="Card Effect Attack")
	AActor* AttackTarget;
	
	UPROPERTY(BlueprintReadWrite,Category="Card Effect Attack")
	float DamageScaleFactor = 0.15f;
	
};
