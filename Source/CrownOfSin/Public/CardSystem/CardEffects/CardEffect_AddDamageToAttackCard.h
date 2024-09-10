
#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "CardEffect_AddDamageToAttackCard.generated.h"

/*손에 든 공격 카드와 더미에 있는 공격 카드의 데미지를 변경합니다(버려도 재설정되지 않음).
 *필수 입력 : 대상 배우(카드), 효과 값(더해지거나 뺄 데미지)*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_AddDamageToAttackCard : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	
	UCardEffect_AddDamageToAttackCard();

protected:
	virtual void BeginPlay() override;

public:
	virtual bool ResolveCardEffect(AActor* TargetActor) override;

};
