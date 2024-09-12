
#pragma once

#include "CoreMinimal.h"
#include "PileComponent.h"
#include "PileDiscardComponent.generated.h"

/**
 * UPileDiscardComponent 클래스는 카드 게임에서 버려진 카드들을 관리하는 버림 더미(Discard Pile)를 담당하는 컴포넌트입니다.
 * 
 * 주요 기능:
 * - 게임 중 버려진 카드들을 저장하고 관리합니다.
 * - 카드를 버림 더미에 추가하거나 필요에 따라 다른 더미로 이동시킬 수 있습니다.
 * 
 * 이 클래스는 게임 내 카드가 더 이상 필요하지 않을 때 버려지는 장소로 사용되며, 버려진 카드들을 추적하는 데 사용됩니다.
 */
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UPileDiscardComponent : public UPileComponent
{
	GENERATED_BODY()

public:
	UPileDiscardComponent();

protected:
	virtual void BeginPlay() override;

public:

};
