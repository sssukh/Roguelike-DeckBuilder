#pragma once

#include "CoreMinimal.h"
#include "CosEnumStruct.h"
#include "GameFramework/Actor.h"
#include "Utilities/CosLog.h"
#include "MinionTrack.generated.h"

class AMinionBase;

UCLASS()
class CROWNOFSIN_API AMinionTrack : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMinionTrack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	/*ToDo:구현해야합니다*/
	bool GetBackMinion(int32& OutBackIndex,AActor*& OutBackTarget)
	{
		COS_LOG_SCREEN(TEXT("GetBackMinion() 함수를 구현해야합니다."));
		return false;
	}

	AMinionBase* AddMinionToTrack(int32 Index, const FMinion& MinionData, bool bSpawnedDuringGameplay)
	{
		COS_LOG_SCREEN(TEXT("AddMinionToTrack() 함수를 구현해야합니다."));
		return nullptr;
	}


	// Member Variables
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MinionTrack")
	AMinionTrack* OpposingTrack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MinionTrack")
	TArray<AMinionBase*> Minions;
};
