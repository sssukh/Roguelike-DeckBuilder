#include "Libraries/FunctionLibrary_Singletons.h"

#include "CardSystem/CardPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/CosLog.h"

ACardPlayer* UFunctionLibrary_Singletons::GetCardPlayer(const UObject* WorldContextObject)
{
	AActor* CardPlayerActor = UGameplayStatics::GetActorOfClass(WorldContextObject, ACardPlayer::StaticClass());

	if (!CardPlayerActor)
	{
		COS_LOG_SCREEN(TEXT("World에서 CardPlayer를 찾지 못했습니다. 스폰하거나 월드에 배치해주세요!!!"));
	}

	return Cast<ACardPlayer>(CardPlayerActor);
}
