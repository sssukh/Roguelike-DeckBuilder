#pragma once

#include "CoreMinimal.h"
#include "ActionBase.h"
#include "Action_UpdateTrack.generated.h"

class AMinionTrack;

UCLASS()
class CROWNOFSIN_API AAction_UpdateTrack : public AActionBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAction_UpdateTrack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Update Track", meta=(ExposeOnSpawn="true"))
	float StartDelay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Update Track", meta=(ExposeOnSpawn="true"))
	AMinionTrack* Track;
};
