#include "CardSystem/CardEffects/CardEffect_AddNewCardToPile.h"

#include "BlueprintGameplayTagLibrary.h"
#include "CardSystem/CardBase.h"
#include "CardSystem/CardPlayer.h"
#include "CardSystem/Piles/PileComponent.h"
#include "Core/DispatcherHubLocalComponent.h"
#include "Core/MinionBase.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "Utilities/CosGameplayTags.h"


UCardEffect_AddNewCardToPile::UCardEffect_AddNewCardToPile()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
	bTargeted = false;
}

void UCardEffect_AddNewCardToPile::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_AddNewCardToPile::ResolveCardEffect(AActor* TargetActor)
{
	// 생성된 카드들을 저장할 배열
	TArray<ACardBase*> SpawnedCards;

	// 카드 게임 인스턴스를 가져옴
	UGameInstance* CardGameInstance = UFunctionLibrary_Singletons::GetCardGameInstance(this);
	if (!CardGameInstance)
		return false;

	// 타겟 액터가 카드일 경우
	if (ACardBase* TargetCard = Cast<ACardBase>(TargetActor))
	{
		// 타겟 카드를 로컬 배열에 추가
		SpawnedCards.Add(TargetCard);
	}
	else
	{
		// 카드 데이터 테이블이 유효하지 않으면 종료
		if (!IsValid(UsedData.DataTable))
			return false;

		// 생성할 카드 수를 결정
		int32 NumCardsToSpawn = FMath::Max(1, EffectValue);
		for (int i = 1; i <= NumCardsToSpawn; ++i)
		{
			// 메인 히어로를 가져옴
			AActor* MainHero;
			IInterface_CardGameInstance::Execute_GetMainHero(CardGameInstance, MainHero);

			// 스폰할 위치 초기화
			FTransform SpawnTransform = FTransform::Identity;

			// 소유자를 설정 (타겟이 히어로일 경우 소유자는 ParentCard의 소유자)
			AActor* OwnerActor = UBlueprintGameplayTagLibrary::HasTag(GameplayTags, CosGameTags::Target_Hero, true)
				                     ? ParentCard->GetOwner()
				                     : MainHero;

			// 새로운 카드를 스폰하고 초기화
			if (ACardBase* NewCard = GetWorld()->SpawnActorDeferred<ACardBase>(
				ACardBase::StaticClass(),
				SpawnTransform,
				OwnerActor,
				nullptr,
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
			{
				// 카드 데이터를 데이터 테이블에서 가져와 설정
				NewCard->CardDataDeck = *UsedData.DataTable->FindRow<FCard>(UsedData.RowName, TEXT(""));
				NewCard->FinishSpawning(SpawnTransform);

				// 스폰된 카드를 로컬 배열에 추가
				SpawnedCards.Add(NewCard);
			}
		}
	}

	// 효과 값이 0 이하일 경우 종료
	if (EffectValue <= 0)
	{
		return false;
	}

	// 미니언을 가져옴
	AMinionBase* Minion = Cast<AMinionBase>(UGameplayStatics::GetActorOfClass(this, AMinionBase::StaticClass()));
	if (!IsValid(Minion))
	{
		// 미니언이 없을 경우, 영웅 ID를 가져와 카드에 설정
		TArray<FMinion> PersistentHeroes = IInterface_CardGameInstance::Execute_GetPersistentHeroesFromInstance(CardGameInstance);

		// 생성된 카드에 영웅의 고유 ID를 설정
		for (ACardBase* Card : SpawnedCards)
		{
			Card->CardDataDeck.OwnerID = PersistentHeroes[0].UniqueID;
		}
	}

	// 대상이 PileComponent의 자식 클래스인지 확인 후 설정
	TSubclassOf<UActorComponent> TargetPileClass;
	if (TargetComponent->IsChildOf(UPileComponent::StaticClass()))
	{
		TargetPileClass = TargetComponent;
	}
	else
	{
		TargetPileClass = UPileComponent::StaticClass();;
	}


	// 카드 플레이어를 가져옴
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	if (!CardPlayer)
	{
		return false;
	}

	// 생성된 카드를 PileComponent에 추가
	for (ACardBase* Card : SpawnedCards)
	{
		// PileComponent를 가져옴
		UActorComponent* PileComponent = CardPlayer->GetComponentByClass(TargetPileClass);

		// Pile에 카드를 추가하는 함수 호출
		IInterface_Pile::Execute_AttemptAddExistingCard(PileComponent, Card);

		// 슬로우 태그가 있는 경우 추가
		FGameplayTagContainer CallTags;
		if (UBlueprintGameplayTagLibrary::HasTag(GameplayTags, CosGameTags::Flag_Slow, true))
		{
			CallTags.AddTag(CosGameTags::Flag_Slow);
		}

		// 카드 생성 이벤트 호출
		Card->DispatcherHubLocal->CallEventWithCallTags(CosGameTags::Event_Card_GenerateCard, Card, nullptr, ECallGlobal::CallAfter, CallTags);
	}

	return true;
}
