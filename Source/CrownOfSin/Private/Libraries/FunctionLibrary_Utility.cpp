#include "Libraries/FunctionLibrary_Utility.h"

#include "CardSystem/CardPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/CosLog.h"

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

void UFunctionLibrary_Utility::SendScreenLogMessage(const UObject* WorldContextObject, FText Message, FColor Color)
{
	if (ACardPlayer* CardPlayer = Cast<ACardPlayer>(UGameplayStatics::GetActorOfClass(WorldContextObject, ACardPlayer::StaticClass())))
		CardPlayer->DisplayScreenLogMessage(Message, Color);
}

FName UFunctionLibrary_Utility::GenerateUniqueObjectName(UObject* Outer, UClass* ObjectClass, const FString& Suffix)
{
	// Get the class name of the object
	FString ClassName = GetNameSafe(ObjectClass);

	// Generate a base name using the class name and optional suffix
	FString BaseName = FString::Printf(TEXT("%s_%s"), *ClassName, *Suffix);

	// Use MakeUniqueObjectName to generate a unique name within the given outer context
	FName UniqueName = MakeUniqueObjectName(Outer, ObjectClass, FName(*BaseName));

	return UniqueName;
}

bool UFunctionLibrary_Utility::CheckObjectImplementsInterface(const UObject* WorldContextObject, UObject* Object, TSubclassOf<UInterface> InterfaceClass)
{
	// 객체가 nullptr인지 확인
	COS_IF_CHECK(Object != nullptr, TEXT("객체가 nullptr입니다."), false);

	// 인터페이스 클래스가 nullptr인지 확인
	COS_IF_CHECK(InterfaceClass != nullptr, TEXT("인터페이스 클래스가 nullptr입니다."), false);

	// 객체가 인터페이스를 구현하고 있는지 확인
	if (Object->GetClass()->ImplementsInterface(InterfaceClass))
	{
		return true;
	}

	FString ObjectName = Object->GetName();
	FString InterfaceName = InterfaceClass->GetName();
	COS_LOG_SCREEN_ERROR(TEXT("%s가 %s를 상속받지 못했습니다."), *ObjectName, *InterfaceName);
	return false;
}
