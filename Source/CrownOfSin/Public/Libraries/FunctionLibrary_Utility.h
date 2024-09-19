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
	UFUNCTION(BlueprintCallable,Category="COS|FunctionLibrary|Utility", meta=(WorldContext="WorldContextObject"))
	static void DisplayWarningIfMultipleSingletons(const UObject* WorldContextObject,TSubclassOf<AActor> InClass);

	UFUNCTION(BlueprintCallable,Category="COS|FunctionLibrary|Utility", meta=(WorldContext="WorldContextObject"))
	static void SendScreenLogMessage(const UObject* WorldContextObject,FText Message, FColor Color);

	UFUNCTION(BlueprintCallable,Category="COS|FunctionLibrary|Utility", meta=(WorldContext="WorldContextObject"))
	static FVector2D LerpVector2D(const FVector2D& A, const FVector2D& B, float Alpha)
	{
		return FVector2D(
			FMath::Lerp(A.X, B.X, Alpha), // X 좌표의 선형 보간
			FMath::Lerp(A.Y, B.Y, Alpha) // Y 좌표의 선형 보간
		);
	}

	UFUNCTION(BlueprintCallable,Category="COS|FunctionLibrary|Utility", meta=(WorldContext="WorldContextObject"))
	static FName GenerateUniqueObjectName(UObject* Outer, UClass* ObjectClass, const FString& Suffix = TEXT("Object"));

};
