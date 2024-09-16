#include "Libraries/FunctionLibrary_Card.h"

#include "CardSystem/CardBase.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "Utilities/CosGameplayTags.h"

FCard UFunctionLibrary_Card::MakeCardStructFromCardData(const FDataTableRowHandle& CardData)
{
	FCard* Card = CardData.DataTable->FindRow<FCard>(CardData.RowName,TEXT("FCard in FunctionLibrary_Card"));

	if (!Card)
	{
		GEngine->AddOnScreenDebugMessage(-1, 9.0f, FColor::Yellow, FString::Printf(TEXT("경고: 라이브러리 함수 MakeCardStructFromData에 잘못된 카드 데이터가 전달되었습니다.")));
		return FCard();
	}

	Card->DataRow = CardData;
	return *Card;
}

void UFunctionLibrary_Card::SpawnAndPlayHeroCard(const UObject* WorldContextObject, const FDataTableRowHandle& CardData, TArray<AActor*> Targets, AActor* OwnerOverride)
{
	UGameInstance* CardGameInstance = UFunctionLibrary_Singletons::GetCardGameInstance(WorldContextObject);
	if (!CardGameInstance) return;

	AActor* MainHero;
	IInterface_CardGameInstance::Execute_GetMainHero(CardGameInstance, MainHero);

	FTransform SpawnTransform = FTransform::Identity;
	AActor* NewOwner = OwnerOverride ? OwnerOverride : MainHero;
	ACardBase* NewCard = WorldContextObject->GetWorld()->SpawnActorDeferred<ACardBase>(ACardBase::StaticClass(), SpawnTransform,
	                                                                                   NewOwner, nullptr,
	                                                                                   ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	NewCard->CardDataDeck = MakeCardStructFromCardData(CardData);
	NewCard->FinishSpawning(SpawnTransform);

	UFunctionLibrary_Event::CallEventInGlobalDispatcherHub(CosGameTags::Event_Card_ModifyInHand, NewCard);

	NewCard->AttemptUseCard(Targets, false, false, false);
}

void UFunctionLibrary_Card::SpawnAndPlayHeroCardUnTargeted(const UObject* WorldContextObject, const FDataTableRowHandle& CardData, AActor* OwnerOverride)
{
	TArray<AActor*> Targets;
	Targets.Add(nullptr);
	SpawnAndPlayHeroCard(WorldContextObject, CardData, Targets, OwnerOverride);
}
