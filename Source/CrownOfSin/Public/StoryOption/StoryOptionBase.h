// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "StoryOptionBase.generated.h"

UCLASS()
class CROWNOFSIN_API AStoryOptionBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStoryOptionBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	// Member Variable
public:
	// 카드 효과 클래스를 나타내는 서브클래스 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect")
	TSubclassOf<UCardEffectComponent> EffectClass;

	// 효과의 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect")
	int32 EffectValue;
	
	// 히어로 애니메이션을 나타내는 태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect")
	FGameplayTag HeroAnim;

	// 관련된 게임플레이 태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect")
	FGameplayTagContainer GameplayTags;
	
	// 타겟 컴포넌트를 나타내는 클래스 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect")
	TSubclassOf<UActorComponent> TargetComponent;

	// 타겟팅 컴포넌트를 나타내는 클래스 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect")
	TSubclassOf<UTargetingComponent> Targeting;

	// 사용된 데이터 테이블 행 핸들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect")
	FDataTableRowHandle UsedData;

	// 고유 식별자
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Effect")
	FString Identifier;
};
