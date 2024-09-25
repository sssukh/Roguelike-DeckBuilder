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
	UFUNCTION(BlueprintCallable, Category="COS|FunctionLibrary|Utility", meta=(WorldContext="WorldContextObject"))
	static void DisplayWarningIfMultipleSingletons(const UObject* WorldContextObject, TSubclassOf<AActor> InClass);

	UFUNCTION(BlueprintCallable, Category="COS|FunctionLibrary|Utility", meta=(WorldContext="WorldContextObject"))
	static void SendScreenLogMessage(const UObject* WorldContextObject, FText Message, FColor Color);

	UFUNCTION(BlueprintCallable, Category="COS|FunctionLibrary|Utility", meta=(WorldContext="WorldContextObject"))
	static FVector2D LerpVector2D(const FVector2D& A, const FVector2D& B, float Alpha);

	UFUNCTION(BlueprintCallable, Category="COS|FunctionLibrary|Utility", meta=(WorldContext="WorldContextObject"))
	static FName GenerateUniqueObjectName(UObject* Outer, UClass* ObjectClass, const FString& Suffix = TEXT("Object"));

	/**
	* 특정 객체가 특정 인터페이스를 구현하고 있는지 확인하는 함수
	* @param WorldContextObject 월드 컨텍스트 객체
	* @param Object 확인할 객체
	* @param InterfaceClass 확인할 인터페이스 클래스
	* @return 인터페이스를 구현하고 있으면 true, 아니면 false
	*/
	UFUNCTION(BlueprintCallable, Category="COS|FunctionLibrary|Utility", meta=(WorldContext="WorldContextObject"))
	static bool CheckObjectImplementsInterface(const UObject* WorldContextObject, UObject* Object, TSubclassOf<UInterface> InterfaceClass);

	/*뷰포트에서 위젯의 현재 위치를 가져옵니다.*/
	UFUNCTION(BlueprintCallable, Category="COS|FunctionLibrary|Utility", meta=(WorldContext="WorldContextObject"))
	static FVector2D GetWidgetTickSpacePosition(UObject* WorldContextObject, UUserWidget* Widget);

	/*세 위젯 사이의 2D 베지어를 계산하고 지정된 알파 포인트(0 = 시작, 1 = 끝)에서 위치와 각도를 반환합니다.*/
	UFUNCTION(BlueprintCallable, Category="COS|FunctionLibrary|Utility", meta=(WorldContext="WorldContextObject"))
	static FVector2D GetPositionAndAngleAtBezierAlphaBetweenWidgets(UObject* WorldContextObject, float Alpha, UUserWidget* StartWidget, UUserWidget* MidWidget, UUserWidget* EndWidget,
																	float& OutAngle);
	
};
