#pragma once

#include "CoreMinimal.h"
#include "PileComponent.h"
#include "PileDrawComponent.generated.h"

/**
 * UPileDrawComponent 클래스는 카드 게임에서 드로우 더미(Draw Pile)를 관리하는 역할을 담당하는 컴포넌트입니다.
 * 
 * 주요 기능:
 * - 플레이어가 카드를 뽑을 수 있도록 드로우 더미를 관리합니다.
 * - 필요 시 버림 더미(Discard Pile)에 있는 카드를 드로우 더미로 섞는 기능을 제공합니다.
 * 
 * 이 클래스는 게임 중 카드를 뽑는 작업을 처리하며, 드로우 더미가 비었을 때 버림 더미에서 카드를 섞어 다시 사용할 수 있게 합니다.
 */
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UPileDrawComponent : public UPileComponent
{
	GENERATED_BODY()

public:
	UPileDrawComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable,Category="Pile Draw Component Event")
	void ReshuffleDiscardIntoDraw();
};
