
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FunctionLibrary_Utility.generated.h"

/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UFunctionLibrary_Utility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/*싱글톤이 여러 개인 경우 경고 표시*/
	UFUNCTION(BlueprintCallable,Category="ATBTT|FunctionLibrary|Utility", meta=(WorldContext="WorldContextObject"))
	static void DisplayWarningIfMultipleSingletons(const UObject* WorldContextObject,TSubclassOf<AActor> InClass);
};
