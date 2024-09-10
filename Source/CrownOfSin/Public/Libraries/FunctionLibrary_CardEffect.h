
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FunctionLibrary_CardEffect.generated.h"

class UStatusComponent;
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UFunctionLibrary_CardEffect : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static int32 AddToStatusCappedToMax(AActor* TargetActor,TSubclassOf<UStatusComponent> StatusClass,int32 Amount,bool bShowSplash,UObject* Payload);
};
