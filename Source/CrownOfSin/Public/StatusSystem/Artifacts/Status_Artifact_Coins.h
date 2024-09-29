
#pragma once

#include "CoreMinimal.h"
#include "Status_Artifact.h"
#include "Status_Artifact_Coins.generated.h"


class ACardPlayer;

UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Artifact_Coins : public UStatus_Artifact
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatus_Artifact_Coins();

UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Status Artifact Coins")
	TObjectPtr<ACardPlayer> CardPlayerRef;
};
