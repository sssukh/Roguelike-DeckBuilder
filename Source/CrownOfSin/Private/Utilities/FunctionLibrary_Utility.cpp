// Temp


#include "Utilities/FunctionLibrary_Utility.h"

#include "Kismet/GameplayStatics.h"
#include "Libraries/CosLog.h"

void UFunctionLibrary_Utility::DisplayWarningIfMultipleSingletons(const UObject* WorldContextObject, TSubclassOf<AActor> InClass)
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(WorldContextObject, InClass, OutActors);

	if (OutActors.Num() > 1)
	{
		FString ClassName = UKismetSystemLibrary::GetClassDisplayName(InClass);
		COS_LOG_SCREEN_ERROR(TEXT("경고: 싱글톤 클래스 %s의 여러 액터가 생성되었습니다. 레벨에는 하나만 존재해야 합니다. "), *ClassName);
	}
}
