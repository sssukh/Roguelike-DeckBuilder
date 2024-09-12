
#pragma once

#include "CoreMinimal.h"
#include "PileComponent.h"
#include "PileShopComponent.generated.h"

/**
 * UPileShopComponent 클래스는 카드 게임에서 상점 카드들을 관리하는 상점 더미(Shop Pile)를 담당하는 컴포넌트입니다.
 * 
 * 주요 기능:
 * - 게임 중 상점에서 구매 가능한 카드들을 관리합니다.
 * - 상점 카드가 판매되는 동안 해당 카드를 추적하고, 필요 시 상점 카드 목록을 갱신할 수 있습니다.
 * 
 * 이 클래스는 상점에서 카드가 구매 가능한 상태를 관리하며, 게임 내 상점 카드 로직을 처리하는 데 사용됩니다.
 */
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UPileShopComponent : public UPileComponent
{
	GENERATED_BODY()

public:
	UPileShopComponent();

protected:
	virtual void BeginPlay() override;

public:
	
};
