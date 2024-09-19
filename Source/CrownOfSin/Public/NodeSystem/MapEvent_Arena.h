#pragma once

#include "CoreMinimal.h"
#include "MapEventComponent.h"
#include "MapEvent_Arena.generated.h"


class UUW_ScreenFade;

UCLASS(ClassGroup=("COS"), Blueprintable, meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UMapEvent_Arena : public UMapEventComponent
{
	GENERATED_BODY()

public:
	UMapEvent_Arena();

public:
	virtual FGameplayTagContainer GetEncounterTags(const FDataTableRowHandle& EncounterTags) override;

	virtual void RunMapEvent(FDataTableRowHandle EventData) override;

	void ChangeLevel(const FDataTableRowHandle& EventData);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Map Event Arena|Config|SetUp")
	TSubclassOf<UUW_ScreenFade> WBP_ScreenFadeClass;
};
