#include "CardSystem/CardEffects/CardEffect_RemoveCardFromPile.h"

#include "CardSystem/CardBase.h"
#include "CardSystem/CardPlayer.h"
#include "CardSystem/Piles/PileComponent.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "UI/UW_Layout_Cos.h"


UCardEffect_RemoveCardFromPile::UCardEffect_RemoveCardFromPile()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UCardEffect_RemoveCardFromPile::BeginPlay()
{
	Super::BeginPlay();
}

bool UCardEffect_RemoveCardFromPile::ResolveCardEffect(AActor* TargetActor)
{
	// 카드 플레이어 객체 가져오기
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	if (!CardPlayer)
	{
		return false; // 카드 플레이어가 없으면 실패
	}

	// TargetActor가 카드 객체인지 확인
	ACardBase* TargetCard = Cast<ACardBase>(TargetActor);
	if (!TargetCard)
	{
		return false; // 카드 객체가 아니면 실패
	}

	// TargetComponent가 PileComponent의 자식인지 확인
	if (!TargetComponent->IsChildOf(UPileComponent::StaticClass()))
	{
		return false; // PileComponent가 아니면 실패
	}

	// PileComponent를 가져옴
	UPileComponent* TargetPileComponent = Cast<UPileComponent>(CardPlayer->GetComponentByClass(TargetComponent));
	if (!TargetPileComponent)
	{
		return false; // PileComponent를 찾지 못하면 실패
	}

	// PileComponent가 IInterface_Pile 인터페이스를 구현하는지 확인
	if (!TargetPileComponent->GetClass()->ImplementsInterface(UInterface_Pile::StaticClass()))
	{
		return false; // 인터페이스를 구현하지 않으면 실패
	}

	// 카드 제거 요청이 실패했는지 확인
	if (!IInterface_Pile::Execute_RequestRemoveCardFromPile(TargetPileComponent, TargetCard))
	{
		return false; // 카드 제거에 실패하면 실패
	}

	// UI 업데이트 (Pile 위젯의 카드 개수 갱신)
	CardPlayer->PlayerUI->UpdatePileWidgetAmount(TargetPileComponent->PileTag, TargetPileComponent->Cards.Num());

	return true; // 모든 과정이 성공적으로 완료됨
}
