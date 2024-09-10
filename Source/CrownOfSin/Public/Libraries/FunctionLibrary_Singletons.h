
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

	// 인터페이스를 상속받았을 경우 UGameInstance를 반환, 그렇지 않으면 nullptr을 반환하는 함수
	UFUNCTION(BlueprintCallable, Category = "Function Library|Singletons", meta = (WorldContext = "WorldContextObject"))
	static UGameInstance* GetCardGameInstance(const UObject* WorldContextObject);


};
