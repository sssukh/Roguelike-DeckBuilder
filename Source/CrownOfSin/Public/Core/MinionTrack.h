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
	AMinionTrack();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	/*ToDo:구현필수*/
	bool GetBackMinion(int32& OutBackIndex, AActor*& OutBackTarget)
	{
		COS_SCREEN(TEXT("GetBackMinion() 함수를 구현해야합니다."));
		return false;
	}

	/*ToDo:구현필수*/
	AMinionBase* AddMinionToTrack(int32 Index, const FMinion& MinionData, bool bSpawnedDuringGameplay)
	{
		COS_SCREEN(TEXT("AddMinionToTrack() 함수를 구현해야합니다."));
		return nullptr;
	}


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MinionTrack")
	AMinionTrack* OpposingTrack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MinionTrack")
	TArray<AMinionBase*> Minions;
};
