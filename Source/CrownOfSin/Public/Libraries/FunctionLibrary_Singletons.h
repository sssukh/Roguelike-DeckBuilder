
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FunctionLibrary_Singletons.generated.h"

class ACardPlayer;
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UFunctionLibrary_Singletons : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Function Library|Singletons", meta=(WorldContext="WorldContextObject"))
	static ACardPlayer* GetCardPlayer(const UObject* WorldContextObject);
};
