#include "CardSystem/CardEffects/CardEffect_AddRandomCardsToPile.h"

#include "CardSystem/CardBase.h"
#include "CardSystem/CardPlayer.h"
#include "CardSystem/ChanceManagerComponent.h"
#include "CardSystem/Piles/PileComponent.h"
#include "CardSystem/Piles/PileHandComponent.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "Utilities/CosGameplayTags.h"


// Sets default values for this component's properties
UCardEffect_AddRandomCardsToPile::UCardEffect_AddRandomCardsToPile()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	bTargeted = false;
	
}


// Called when the game starts
void UCardEffect_AddRandomCardsToPile::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_AddRandomCardsToPile::ResolveCardEffect(AActor* TargetActor)
{
	// 타겟이 Pile 컴포넌트인지 확인하고 적절한 클래스 설정
	TSubclassOf<UPileComponent> TargetPileClass = GetTargetPileClass(TargetComponent);

	// CardPlayer 인스턴스 가져오기
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	if (!CardPlayer)
		return false;

	// 랜덤 카드를 생성하여 Pile에 추가
	return AddRandomCardsToPile(CardPlayer, TargetPileClass);
}

TSubclassOf<UPileComponent> UCardEffect_AddRandomCardsToPile::GetTargetPileClass(UClass* InTargetComponent) const
{
	return InTargetComponent->IsChildOf(UPileComponent::StaticClass()) ? InTargetComponent : UPileHandComponent::StaticClass();
}


bool UCardEffect_AddRandomCardsToPile::AddRandomCardsToPile(ACardPlayer* CardPlayer, TSubclassOf<UPileComponent> TargetPileClass)
{
	// 랜덤 카드를 가져오기
	const TArray<FCard>& RandomCards = CardPlayer->ChanceManagerComponent->GetTrueRandomCardsOfAnyRarity(EffectValue);
	TArray<ACardBase*> LocalCards;

	// 각 랜덤 카드에 대해 처리
	for (const FCard& RandomCard : RandomCards)
	{
		ACardBase* NewCard = SpawnNewCard(RandomCard);
		if (!NewCard)
			continue;

		LocalCards.Add(NewCard);

		// TargetPileComponent 가져오기
		UActorComponent* TargetPileComponent = CardPlayer->GetComponentByClass(TargetPileClass);

		// Pile 인터페이스를 구현하는지 확인
		if (TargetPileComponent && TargetPileComponent->GetClass()->ImplementsInterface(UInterface_Pile::StaticClass()))
		{
			// Pile에 카드 추가 시도
			if (IInterface_Pile::Execute_AttemptAddExistingCard(TargetPileComponent, NewCard))
			{
				NewCard->CallLocalEventOnCard(CosGameTags::Event_Card_GenerateCard, ECallGlobal::CallAfter);
			}
		}
	}
	return true;
}

ACardBase* UCardEffect_AddRandomCardsToPile::SpawnNewCard(const FCard& RandomCard)
{
	FTransform SpawnTransform = FTransform::Identity;

	ACardBase* NewCard = GetWorld()->SpawnActorDeferred<ACardBase>(
		ACardBase::StaticClass(),
		SpawnTransform,
		ParentCard->GetOwner(),
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	if (NewCard)
	{
		NewCard->CardDataDeck = RandomCard;
		NewCard->FinishSpawning(SpawnTransform);
	}

	return NewCard;
}
