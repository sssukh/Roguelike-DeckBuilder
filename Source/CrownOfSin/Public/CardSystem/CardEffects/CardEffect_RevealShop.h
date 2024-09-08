
#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "CardEffect_RevealShop.generated.h"

/*레이아웃 위젯에 상점 위젯이 표시되도록 합니다.*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_RevealShop : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	UCardEffect_RevealShop();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual bool ResolveCardEffect(AActor* TargetActor) override;
};
