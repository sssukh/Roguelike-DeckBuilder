
#pragma once

#include "CoreMinimal.h"
#include "AttackPatternComponent.h"
#include "AttackPatternComponent_SpiderQueen.generated.h"


UCLASS(ClassGroup=("COS|Combat"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UAttackPatternComponent_SpiderQueen : public UAttackPatternComponent
{
	GENERATED_BODY()

public:
	UAttackPatternComponent_SpiderQueen();

protected:
	virtual void BeginPlay() override;

public:
	virtual ACardBase* GetNextCard() override;
	
	
};
