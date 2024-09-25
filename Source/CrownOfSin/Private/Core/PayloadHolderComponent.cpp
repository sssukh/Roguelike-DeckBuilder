#include "Core/PayloadHolderComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Utilities/CosLog.h"


// Sets default values for this component's properties
UPayloadHolderComponent::UPayloadHolderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}

void UPayloadHolderComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UPayloadHolderComponent::AddPayload(const FString& Identifier, UObject* Payload)
{
	// 이미 동일한 식별자로 페이로드가 존재하는지 확인
	if (Payloads.Contains(Identifier))
	{
		FString ExistingPayloadName = UKismetSystemLibrary::GetDisplayName(Payload);
		FString ComponentName = UKismetSystemLibrary::GetDisplayName(this);
		FString WarningMessage = FString::Printf(
			TEXT("경고: 페이로드 %s을(를) 식별자 %s로 PayloadHolderComponent %s에 추가하려고 합니다. 기존 페이로드를 덮어씁니다."),
			*ExistingPayloadName, *Identifier, *ComponentName
		);
		COS_LOG_SCREEN(TEXT("%s"), *WarningMessage);
	}

	// 페이로드 추가 또는 덮어쓰기
	Payloads.Add(Identifier, Payload);
}

bool UPayloadHolderComponent::AccessPayload(const FString& Identifier, bool bDestroyAfterAccess, UObject*& OutPayload)
{
	// 주어진 식별자로 페이로드가 존재하는지 확인
	if (Payloads.Contains(Identifier))
	{
		// 페이로드를 로컬 변수에 할당
		UObject* PayloadToAccess = Payloads[Identifier];

		// 페이로드를 삭제하고 반환할 경우
		if (bDestroyAfterAccess)
		{
			Payloads.Remove(Identifier);
			OutPayload = PayloadToAccess;
			return true;
		}

		// 페이로드에 접근만 하고 유지할 경우
		OutPayload = PayloadToAccess;
		return true;
	}

	// 페이로드를 찾지 못한 경우
	OutPayload = nullptr;
	return false;
}
