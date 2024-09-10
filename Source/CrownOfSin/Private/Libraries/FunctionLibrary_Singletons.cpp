#include "Libraries/FunctionLibrary_Singletons.h"

#include "CardSystem/CardPlayer.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/CosLog.h"

ACardPlayer* UFunctionLibrary_Singletons::GetCardPlayer(const UObject* WorldContextObject)
{
	AActor* CardPlayerActor = UGameplayStatics::GetActorOfClass(WorldContextObject, ACardPlayer::StaticClass());

	if (!CardPlayerActor)
	{
		COS_LOG_SCREEN(TEXT("World에서 CardPlayer를 찾지 못했습니다. 스폰하거나 월드에 배치해주세요!!!"));
		return nullptr;
	}

	if (!CardPlayerActor->GetClass()->ImplementsInterface(UInterface_CardTarget::StaticClass()))
	{
		COS_LOG_SCREEN(TEXT("게임 인스턴스가 UInterface_CardTarget을 상속받지 않았습니다"));
		return nullptr;
	}

	return Cast<ACardPlayer>(CardPlayerActor);
}

UGameInstance* UFunctionLibrary_Singletons::GetCardGameInstance(const UObject* WorldContextObject)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	if (!GameInstance)
	{
		return nullptr;
	}

	if (!GameInstance->GetClass()->ImplementsInterface(UInterface_CardGameInstance::StaticClass()))
	{
		COS_LOG_SCREEN(TEXT("게임 인스턴스가 UInterface_CardGameInstance를 상속받지 않았습니다"));
		return nullptr;
	}

	// 게임 인스턴스가 유효하고 인터페이스를 구현한 경우 해당 인스턴스를 반환
	return GameInstance;
}
