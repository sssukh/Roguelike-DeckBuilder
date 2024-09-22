#pragma once

#include "CoreMinimal.h"
#include "ActionBase.h"
#include "Action_Appear.generated.h"

/*입력에 따라 퍼펫이 나타나거나 사라지도록 호출합니다. 인형은 이러한 이벤트를 애니메이션화/표시하는 방법을 처리합니다.*/
UCLASS()
class CROWNOFSIN_API AAction_Appear : public AActionBase
{
	GENERATED_BODY()

public:
	AAction_Appear();

protected:
	virtual void BeginPlay() override;

public:
	virtual void PlayAction_Implementation() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Appear", meta=(ExposeOnSpawn="true"))
	bool bDisappear = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Appear", meta=(ExposeOnSpawn="true"))
	float FadeSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Appear", meta=(ExposeOnSpawn="true"))
	AActor* Puppet;
};
