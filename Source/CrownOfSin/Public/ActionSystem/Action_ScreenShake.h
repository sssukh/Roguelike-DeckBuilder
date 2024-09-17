
#pragma once

#include "CoreMinimal.h"
#include "ActionBase.h"
#include "Action_ScreenShake.generated.h"

UCLASS()
class CROWNOFSIN_API AAction_ScreenShake : public AActionBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAction_ScreenShake();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Action ScreenShake",meta=(ExposeOnSpawn="true"))
	float Scale =1.0f;

	//ToDo:기본값 설정해야합니다.
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Action ScreenShake",meta=(ExposeOnSpawn="true"))
	TSubclassOf<UCameraShakeBase> ShakeClass;
};
