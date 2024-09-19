#pragma once

#include "CoreMinimal.h"
#include "ActionBase.h"
#include "Action_ScreenShake.generated.h"

/*대기열의 플레이어 카메라 관리자에서 입력 화면 흔들림을 트리거합니다.*/
UCLASS()
class CROWNOFSIN_API AAction_ScreenShake : public AActionBase
{
	GENERATED_BODY()

public:
	AAction_ScreenShake();

protected:
	virtual void BeginPlay() override;

public:
	virtual void PlayAction_Implementation() override;

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action ScreenShake", meta=(ExposeOnSpawn="true"))
	float Scale = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action ScreenShake", meta=(ExposeOnSpawn="true"))
	TSubclassOf<UCameraShakeBase> ShakeClass;
};
