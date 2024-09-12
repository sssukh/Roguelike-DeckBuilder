
#pragma once

#include "CoreMinimal.h"
#include "PileComponent.h"
#include "PileVoidComponent.generated.h"

/**
 * UPileVoidComponent 클래스는 카드 게임에서 무효화된 카드들을 관리하는 무효화 더미(Void Pile)를 담당하는 컴포넌트입니다.
 * 
 * 주요 기능:
 * - 게임 중 무효화된 카드들을 저장하고 관리합니다.
 * - 무효화된 카드는 게임 내에서 더 이상 사용되지 않으며, 추적을 위해 이 더미에 보관됩니다.
 * 
 * 이 클래스는 카드가 게임에서 무효화되거나 제거된 상태를 관리하며, 무효화된 카드가 게임 흐름에 영향을 미치지 않도록 처리하는 데 사용됩니다.
 */
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UPileVoidComponent : public UPileComponent
{
	GENERATED_BODY()

public:
	UPileVoidComponent();

protected:
	virtual void BeginPlay() override;

public:
	
};
