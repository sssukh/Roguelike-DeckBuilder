#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NodeMapMode.generated.h"

/**
 * Node Map 레벨의 게임 모드
 */
UCLASS()
class CROWNOFSIN_API ANodeMapMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANodeMapMode(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
};
