#pragma once

#include "CoreMinimal.h"
#include "Status_Artifact.h"
#include "Status_Artifact_CardReward.generated.h"

/*클릭하면 카드 보상이 포함된 Discover 카드가 재생됩니다. 보상 화면에 사용됩니다.*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Artifact_CardReward : public UStatus_Artifact
{
	GENERATED_BODY()

public:
	UStatus_Artifact_CardReward();

protected:
	virtual void BeginPlay() override;

public:
	virtual int32 AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, bool bRefreshAppearance, UObject* InPayload) override;
	
	virtual bool Interact_Implementation(const FGameplayTagContainer& Tags) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Status Artifact Card Reward")
	FDataTableRowHandle DefaultRewardData;
	
	UPROPERTY(BlueprintReadWrite, Category="Status Artifact Card Reward")
	FDataTableRowHandle CardRewardData;
};
