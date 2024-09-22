#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Interfaces/Interface_CardPuppet.h"
#include "PuppetAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UPuppetAnimInstance : public UAnimInstance, public IInterface_CardPuppet
{
	GENERATED_BODY()
};
