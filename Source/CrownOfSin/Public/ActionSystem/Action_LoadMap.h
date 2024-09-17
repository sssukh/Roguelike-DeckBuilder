#pragma once

#include "CoreMinimal.h"
#include "ActionBase.h"
#include "Action_LoadMap.generated.h"

class UUW_ScreenFade;


/*대기열에 입력 레벨을 로드합니다.*/
UCLASS()
class CROWNOFSIN_API AAction_LoadMap : public AActionBase
{
	GENERATED_BODY()

public:
	AAction_LoadMap();

protected:
	virtual void BeginPlay() override;

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Load Map|Config|SetUp")
	TSubclassOf<UUW_ScreenFade> WBP_ScreenFadeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Load Map", meta=(ExposeOnSpawn="true"))
	FString Level;
};
