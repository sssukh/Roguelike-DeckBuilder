
#pragma once

#include "CoreMinimal.h"
#include "PileComponent.h"
#include "PileExhaustComponent.generated.h"

/**
 * UPileExhaustComponent 클래스는 카드 게임에서 소진된 카드들을 관리하는 소진 더미(Exhaust Pile)를 담당하는 컴포넌트입니다.
 * 
 * 주요 기능:
 * - 게임 중 사용된 카드들을 소진 더미에 저장하고, 다시 사용되지 않도록 관리합니다.
 * - 버림 더미나 드로우 더미와 달리, 소진된 카드는 게임이 끝날 때까지 재사용되지 않습니다.
 * 
 * 이 클래스는 카드가 완전히 소진된 상태를 추적하며, 소진된 카드들이 다시 게임에 사용되지 않도록 관리하는 역할을 합니다.
 */
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UPileExhaustComponent : public UPileComponent
{
	GENERATED_BODY()

public:
	UPileExhaustComponent();

protected:
	virtual void BeginPlay() override;

public:
	
};
