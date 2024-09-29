
#pragma once

#include "CoreMinimal.h"
#include "ActionBase.h"
#include "Action_CardRewardScreen.generated.h"

class ACardBase;

UCLASS()
class CROWNOFSIN_API AAction_CardRewardScreen : public AActionBase
{
	GENERATED_BODY()

public:
	AAction_CardRewardScreen();

	virtual void PlayAction_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Action Card Reward Screen")
	void ContinueActionOnCardSelect(bool bSkipped, ACardBase* InCard);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Card Reward Screen")
	TArray<ACardBase*> CardOptions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Card Reward Screen")
	FText ScreenText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Card Reward Screen")
	bool bAllowSkip;
};
