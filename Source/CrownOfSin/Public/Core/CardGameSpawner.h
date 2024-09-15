#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CardGameSpawner.generated.h"

/*
 * ACardGameSpawner 클래스는 카드 전투에 필요한 액터들을 생성하기 위해 사용되는 스포너 클래스입니다.
 * 이 클래스는 Turn Manager, Card Player, 그리고 기타 카드 전투와 관련된 모든 액터를 Arena 맵에서 생성하는 역할을 담당합니다.
 */
UCLASS()
class CROWNOFSIN_API ACardGameSpawner : public AActor
{
	GENERATED_BODY()

public:
	ACardGameSpawner();

protected:
	virtual void BeginPlay() override;

public:
	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	// 이 스포너의 기본 루트 씬 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card Game Spawner|Component")
	USceneComponent* DefaultSceneRoot;

	// 생성할 액터의 순서를 정의하는 배열. 배열에 정의된 순서대로 액터들이 생성됩니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Card Game Spawner|Config|SetUp")
	TArray<TSubclassOf<AActor>> SpawnOrderActors;
};
