#pragma once

#include "CoreMinimal.h"
#include "ActionBase.h"
#include "Action_RewardScreen.generated.h"

/*카드 보상 옵션을 표시하는 보상 화면을 표시합니다.*/
UCLASS()
class CROWNOFSIN_API AAction_RewardScreen : public AActionBase
{
	GENERATED_BODY()

public:
	AAction_RewardScreen();

protected:
	virtual void BeginPlay() override;

public:
	virtual void PlayAction_Implementation() override;
};
