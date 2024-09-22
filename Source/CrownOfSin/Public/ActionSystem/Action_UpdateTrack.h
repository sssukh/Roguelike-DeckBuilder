#pragma once

#include "CoreMinimal.h"
#include "ActionBase.h"
#include "Action_UpdateTrack.generated.h"

class AMinionTrack;

/*모든 하수인을 대기열의 적절한 위치로 밀어넣도록 입력 트랙에 지시합니다.*/
UCLASS()
class CROWNOFSIN_API AAction_UpdateTrack : public AActionBase
{
	GENERATED_BODY()

public:
	AAction_UpdateTrack();

protected:
	virtual void BeginPlay() override;

public:
	virtual void PlayAction_Implementation() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Update Track", meta=(ExposeOnSpawn="true"))
	float StartDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Update Track", meta=(ExposeOnSpawn="true"))
	AMinionTrack* Track;
};
