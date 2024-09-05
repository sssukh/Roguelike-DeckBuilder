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
	UFUNCTION(BlueprintCallable, Category = "Function Library|Array")
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


	template <typename T>
	UFUNCTION(BlueprintCallable, Category = "Function Library|Array")
	static T GetRandomElementFromArray(const TArray<T>& TargetArray, int32& OutRandomIndex)
	{
		// 배열이 비어 있는지 확인
		if (TargetArray.Num() == 0)
		{
			OutRandomIndex = INDEX_NONE; // 배열이 비어있으면 유효하지 않음을 반환
			return T(); // 기본값 반환
		}

		// 배열에서 무작위 인덱스 선택
		OutRandomIndex = FMath::RandRange(0, TargetArray.Num() - 1);
		return TargetArray[OutRandomIndex];
	}
};
