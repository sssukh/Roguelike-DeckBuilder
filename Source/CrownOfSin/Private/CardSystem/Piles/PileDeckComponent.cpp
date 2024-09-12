#include "CardSystem/Piles/PileDeckComponent.h"

#include "CardSystem/CardBase.h"
#include "CardSystem/Piles/PileDrawComponent.h"
#include "Core/MinionBase.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "Utilities/CosGameplayTags.h"


UPileDeckComponent::UPileDeckComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	PileTag = CosGameTags::Pile_Deck;
	FriendlyName = FText::FromString(FString(TEXT("Deck")));
}


void UPileDeckComponent::BeginPlay()
{
	Super::BeginPlay();

	// 다음 틱에 실행될 로직을 정의하여 0.0초 뒤에 덱을 설정하는 작업을 수행합니다.
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		// 게임 인스턴스에서 덱 데이터를 가져옵니다.
		UGameInstance* CardGameInstance = UFunctionLibrary_Singletons::GetCardGameInstance(this);
		if (!CardGameInstance) return; // 게임 인스턴스가 유효하지 않으면 종료

		// 덱 데이터를 게임 인스턴스에서 가져오려 시도합니다.
		TArray<FCard> DeckCards;
		if (IInterface_CardGameInstance::Execute_GetDeckFromInstance(CardGameInstance, DeckCards))
		{
			// 덱 데이터를 기반으로 카드를 생성하고 덱에 추가합니다.
			for (const FCard& DeckCard : DeckCards)
			{
				// 소유자의 위치와 회전값을 가져와 카드를 생성할 준비를 합니다.
				FTransform SpawnTransform = GetOwner()->GetActorTransform();

				if (ACardBase* NewCard = GetWorld()->SpawnActorDeferred<ACardBase>(ACardBase::StaticClass(),
				                                                                   SpawnTransform,
				                                                                   nullptr,
				                                                                   nullptr,
				                                                                   ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
				{
					// 생성된 카드에 덱 데이터를 할당하고 생성 완료를 호출합니다.
					NewCard->CardDataDeck = DeckCard;
					NewCard->FinishSpawning(SpawnTransform);

					// 생성된 카드를 덱에 추가하려고 시도합니다.
					IInterface_Pile::Execute_AttemptAddExistingCard(this, NewCard);
				}
			}

			// 카드를 드로우 더미로 설정하고 덱 초기화 완료 이벤트를 호출합니다.
			SetupCardsInDrawPile();
			UFunctionLibrary_Event::CallEventInGlobalDispatcherHub(CosGameTags::Event_DeckSetupDone, this);
		}
		else
		{
			// 덱 데이터를 찾지 못했을 경우, 데이터 테이블에서 덱 정보를 로드합니다.
			FDeck* FoundDeck = DeckData.DataTable->FindRow<FDeck>(DeckData.RowName,TEXT(""));
			if (!FoundDeck) return;

			// 데이터 테이블의 덱 카드 데이터를 기반으로 새 카드를 생성하고 덱에 추가합니다.
			for (const FDataTableRowHandle& Card : FoundDeck->Cards)
			{
				ACardBase* SpawnCard;
				Execute_SpawnAndAddNewCard(this, Card, SpawnCard);
			}

			// 카드를 드로우 더미로 설정합니다.
			SetupCardsInDrawPile();
		}
	});
}

void UPileDeckComponent::SetupCardsInDrawPile()
{
	// 드로우 더미 컴포넌트를 가져옵니다.
	UPileDrawComponent* PileDrawComponent = Cast<UPileDrawComponent>(GetOwner()->GetComponentByClass(UPileDrawComponent::StaticClass()));
	if (!PileDrawComponent) return;

	// 드로우 더미에 현재 덱의 카드를 할당합니다.
	IInterface_Pile::Execute_AssignExistingCards(PileDrawComponent, Cards);

	// 현재 씬에 있는 모든 Minion 객체를 가져옵니다.
	TArray<AActor*> MinionActors;
	UGameplayStatics::GetAllActorsOfClass(this, AMinionBase::StaticClass(), MinionActors);

	// 덱에 있는 각 카드를 순회합니다.
	for (ACardBase* Card : Cards)
	{
		// 모든 Minion 객체를 순회하여 각 카드를 소유자와 연결합니다.
		for (AActor* MinionActor : MinionActors)
		{
			AMinionBase* Minion = Cast<AMinionBase>(MinionActor);
			if (!Minion) continue; // Minion 객체가 유효하지 않으면 건너뜁니다.

			// Minion의 고유 ID와 카드의 소유자 ID가 일치하면 카드를 해당 Minion에 할당합니다.
			if (UKismetStringLibrary::EqualEqual_StrStr(Minion->MinionData.UniqueID, Card->GetCardOwnerID(ECardDataType::Deck)))
			{
				Card->SetOwner(Minion);
			}
		}
	}

	// 덱 설정 완료 이벤트를 전역 이벤트 디스패처에서 호출합니다.
	UFunctionLibrary_Event::CallEventInGlobalDispatcherHub(CosGameTags::Event_DeckSetupDone, this);
}
