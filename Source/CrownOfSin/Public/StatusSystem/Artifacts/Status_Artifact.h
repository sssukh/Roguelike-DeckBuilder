
#pragma once

#include "CoreMinimal.h"
#include "StatusSystem/StatusComponent.h"
#include "Status_Artifact.generated.h"

/*모든 아티팩트 상태에 대한 상위 상태입니다.
 *상태 변수에 대한 몇 가지 적절한 기본값을 설정하고 보상 화면에서 클릭할 때 아티팩트 표시줄에 아티팩트를 추가하기 위한 Interact 코드를 포함합니다.*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Artifact : public UStatusComponent
{
	GENERATED_BODY()

public:
	UStatus_Artifact();

protected:
	virtual void BeginPlay() override;

public:
	virtual int32 AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, bool bRefreshAppearance, UObject* InPayload) override;
	
	/*========================================================================================
	*	IInterface_Utility
	=========================================================================================*/
public:
	virtual bool Interact_Implementation(const FGameplayTagContainer& Tags) override;
	
};
