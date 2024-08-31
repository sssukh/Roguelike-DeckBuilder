#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActionBase.generated.h"

UCLASS()
class CROWNOFSIN_API AActionBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AActionBase();

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
	/*PlayAction 종료와 다음 작업을 재생하려는 시점 사이의 초 단위 지연입니다. 값이 0이면 지연 시간이 표시됩니다. 0 미만의 지연은 지연을 제공하지 않습니다.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action Base", meta=(ExposeOnSpawn="true"))
	float EndDelay = -1.0f;
};
