// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/MinionTrack.h"
#include "Core/CosEnumStruct.h"
#include "TargetingComponent.generated.h"

class AMinionTrack;
class ACardBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputTargetsReceived, TArray<AActor*>, Targets);



UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTargetingComponent();

public:
	// 입력한 매개변수에 기반해 유효한 Target 배열을 찾고 가져옵니다.
	// 자식 클래스에서 override해서 재정의합니다.
	UFUNCTION(BlueprintCallable, Category="Targeting Component")
	virtual bool FindValidTargets(TArray<AActor*>& SpecifiedTargets, const FCardEffect& CardEffect, AEffectActor* Card, bool bPreview, TArray<AActor*>& OutValidTargets);

	// 맵 상의 모든 미니언들을 찾고 매개변수에 입력한 게임플레이 태그들을 하나라도 갖고있는 미니언들을 가져옵니다.
	UFUNCTION(BlueprintCallable, Category="Targeting Component")
	TArray<AActor*> FindAllMinionsWithMatchingTags(FGameplayTagContainer GameplayTags);

	// 카드 소유자가 서있는 MinionTrack과 Opposing MinionTrack을 찾고 가져옵니다.
	// 카드소유자는 미니언이고 track에 있어야 합니다.
	UFUNCTION(BlueprintCallable, Category="Targeting Component")
	void FindMinionTracks(AEffectActor* Card, AMinionTrack*& OwnerTrack, AMinionTrack*& OpposingTrack);

	// 카드 소유자의 반대 트랙에 있는 모든 미니언들이 담긴 배열을 반환합니다.
	UFUNCTION(BlueprintCallable, Category="Targeting Component")
	bool GetAllMinionsOnOpposingTrack(AEffectActor* Card, TArray<AMinionBase*>& Minions);

	bool CheckTargetHasGameplayTag(AActor* Target, const FCardEffect& CardEffect);
	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bRequiresInput;

	/*========================================================================================
	*	Delegates
	=========================================================================================*/
	FOnInputTargetsReceived OnInputTargetsReceived;
};
