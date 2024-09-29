// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/CosEnumStruct.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FunctionLibrary_Card.generated.h"

/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UFunctionLibrary_Card : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Function Library|Card")
	static FCard MakeCardStructFromCardData(const FDataTableRowHandle& CardData);

	UFUNCTION(BlueprintCallable, Category="Function Library|Card", meta = (WorldContext = "WorldContextObject"))
	static void SpawnAndPlayHeroCard(const UObject* WorldContextObject, const FDataTableRowHandle& CardData, const TArray<AActor*>& Targets, AActor* OwnerOverride);

	/**
 	 * 타겟이 없는 히어로 카드를 생성하고 실행하는 함수.
 	 * 타겟이 필요 없는 상황에서 호출되며, 타겟 리스트에 명시적으로 null 값을 추가하여 처리합니다.
 	 *
 	 * @param WorldContextObject  월드 컨텍스트 객체, 카드를 스폰할 위치 등을 결정
 	 * @param CardData   생성할 카드의 데이터 핸들
 	 * @param OwnerOverride      카드의 소유자를 오버라이드할 액터
 	 */
	UFUNCTION(BlueprintCallable, Category="Function Library|Card", meta = (WorldContext = "WorldContextObject"))
	static void SpawnAndPlayHeroCardUnTargeted(const UObject* WorldContextObject, const FDataTableRowHandle& CardData, AActor* OwnerOverride);
};
