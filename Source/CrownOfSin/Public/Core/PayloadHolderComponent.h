#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PayloadHolderComponent.generated.h"


UCLASS(ClassGroup=("COS|Core"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UPayloadHolderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPayloadHolderComponent();

protected:
	virtual void BeginPlay() override;

public:
	/**
	 * @brief 주어진 식별자(Identifier)와 함께 페이로드(Payload)를 추가하는 함수입니다.
	 * 이미 존재하는 식별자에 페이로드를 추가할 경우 경고 메시지를 출력하고 기존 데이터를 덮어씁니다.
	 * 
	 * @param Identifier 페이로드를 구분하는 고유 식별자입니다.
	 * @param Payload 추가할 페이로드 객체입니다.
	 */
	UFUNCTION(BlueprintCallable, Category="Payload Holder Event")
	void AddPayload(const FString& Identifier, UObject* Payload);

	/**
 	 * @brief 주어진 식별자로 페이로드에 접근하는 함수입니다.
 	 * 성공적으로 접근하면 해당 페이로드를 반환하고, 선택적으로 페이로드를 제거합니다.
 	 * 
 	 * @param Identifier 접근할 페이로드의 고유 식별자입니다.
 	 * @param bDestroyAfterAccess 페이로드에 접근 후 해당 데이터를 제거할지 여부입니다.
 	 * @param OutPayload 반환될 페이로드 객체의 참조입니다.
 	 * @return 접근 성공 여부를 반환합니다.
 	 */
	UFUNCTION(BlueprintCallable, Category="Payload Holder Event")
	bool AccessPayload(const FString& Identifier, bool bDestroyAfterAccess, UObject*& OutPayload);

public:
	UPROPERTY(BlueprintReadWrite, Category="Payload Holder")
	TMap<FString, UObject*> Payloads;
};
