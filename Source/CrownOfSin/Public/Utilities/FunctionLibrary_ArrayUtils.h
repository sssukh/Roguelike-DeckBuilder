// Temp

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FunctionLibrary_ArrayUtils.generated.h"

/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UFunctionLibrary_ArrayUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	template <typename T>
	UFUNCTION(BlueprintCallable, Category = "Array")
	static void ShuffleArray(UPARAM(ref) TArray<T>& TargetArray)
	{
		int32 LastIndex = TargetArray.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				TargetArray.Swap(i, Index);
			}
		}
	}
};
