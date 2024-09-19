#pragma once

#include "CoreMinimal.h"
#include "CosEnumStruct.h"
#include "GameFramework/Actor.h"
#include "MinionTrack.generated.h"

class UBoxComponent;
class UGameplayTagComponent;
class AMinionBase;


class UTimelineComponent; // 타임라인 컴포넌트를 위한 클래스 선언


/*
 * 순서가 지정된 목록에 미니언을 보관합니다. 퍼펫의 간격과 이동을 처리합니다.
 */
UCLASS()
class CROWNOFSIN_API AMinionTrack : public AActor
{
	GENERATED_BODY()

public:
	AMinionTrack();

protected:
	virtual void BeginPlay() override;

public:
	
	/**
	 * 트랙에 새로운 미니언을 추가하고, 미니언이 트랙에서 올바르게 위치하도록 설정합니다.
	 * 게임플레이 중에 스폰된 경우 해당 액션을 큐에 추가하고, 새로운 미니언을 적절한 위치에 배치한 후 반환합니다.
	 * 
	 * @param Index 미니언을 추가할 트랙의 인덱스.
	 * @param InMinionData 추가할 미니언의 데이터.
	 * @param bSpawnedDuringGameplay 게임플레이 중 스폰 여부.
	 * @return 생성된 미니언의 포인터.
	 */
	UFUNCTION(BlueprintCallable, Category="Minion Track")
	AMinionBase* AddMinionToTrack(int32 Index, const FMinion& InMinionData, bool bSpawnedDuringGameplay);

protected:
	/**
	 * 주어진 액터(InTarget)가 파괴될 때 RemoveActorIfDestroyed 함수가 호출되도록 바인딩합니다.
	 * @param InTarget 파괴될 때 이벤트를 바인딩할 대상 액터.
	 */	
	UFUNCTION()
	void BindTargetRemoval(AActor* InTarget);

	UFUNCTION()
	void SlideMinionsToPositions(float InStartDelay);
	
public:
	UFUNCTION(BlueprintPure, Category="Minion Track")
	bool GetFrontMinion(AActor*& OutFoundTarget, int32& OutTargetIndex);

	UFUNCTION(BlueprintPure, Category="Minion Track")
	bool GetBackMinion(AActor*& OutBackTarget, int32& OutBackIndex);

	/**
	 * 주어진 인덱스(TrackIndex)에 해당하는 트랙 상의 위치를 반환하는 함수.
	 * 미니언들의 위치를 계산하고, 특정 스폰 위치 또는 이동할 위치를 반환한다.
	 * 
	 * @param TrackIndex 미니언의 트랙 상 인덱스.
	 * @param bNewSpawn 새로운 미니언 스폰 여부 (true면 스폰 위치를 포함하여 계산).
	 * @return 주어진 인덱스에 해당하는 월드 공간에서의 위치(FVector).
	 */
	UFUNCTION(BlueprintPure, Category="Minion Track")
	FVector GetTrackLocationAtIndex(int32 TrackIndex, bool bNewSpawn);

	/**
	 * 주어진 미니언(InMinion)의 트랙 내 위치에 기반하여 이니셔티브 값을 계산하여 반환.
	 * 트랙에서 뒤에 위치한 미니언일수록 높은 이니셔티브 값을 가집니다.
	 * 
	 * @param InMinion 이니셔티브 값을 계산할 미니언.
	 * @return 이니셔티브 값 (미니언의 위치와 InitiativeModifier를 기반으로 계산).
	 */	
	UFUNCTION(BlueprintCallable, Category="Minion Track")
	float GetInitiativeFromTrack(AMinionBase* InMinion);



	/*========================================================================================
	*	On Call back
	=========================================================================================*/
protected:
	/**
	 * 미니언이 파괴되었을 때 트랙에서 해당 미니언을 제거하고, 트랙 업데이트 작업을 큐에 추가합니다.
	 * @param DestroyedActor 파괴된 액터 (해당 액터가 AMinionBase 타입일 때만 제거).
	 */	
	UFUNCTION()
	void RemoveActorIfDestroyed(AActor* DestroyedActor);

	// 지연 후 실행될 콜백 함수
	UFUNCTION()
	void OnSlideMinionsComplete();

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MinionTrack|Component")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Minion Track|Component")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Minion Track|Component")
	UGameplayTagComponent* GameplayTagComponent;

	// 블루프린트 타임라인을 C++에서 사용할 멤버 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Minion Track|Component")
	UTimelineComponent* SlideTimeline;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minion Track")
	int32 NumMinions = 2;
	
	UPROPERTY(BlueprintReadWrite, Category="Minion Track")
	int32 ForwardOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minion Track")
	float Spacing = 80.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minion Track", meta=(ExposeOnSpawn="true"))
	float InitiativeModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minion Track", meta=(ExposeOnSpawn="true"))
	FGameplayTagContainer InitialGameplayTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minion Track", meta=(ExposeOnSpawn="true"))
	AMinionTrack* OpposingTrack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Minion Track", meta=(ExposeOnSpawn="true"))
	FDataTableRowHandle MinionData;

	UPROPERTY(BlueprintReadWrite, Category="Minion Track")
	TArray<FVector> PuppetLocationsPreSlide;

	UPROPERTY(BlueprintReadWrite, Category="Minion Track")
	TArray<AMinionBase*> Minions;
};
