#include "CombatSystem/AttackPatternComponent.h"

#include "CardSystem/CardBase.h"
#include "Core/CosEnumStruct.h"
#include "Interfaces/Interface_CardTarget.h"
#include "StatusSystem/Status_AttackPattern.h"


UAttackPatternComponent::UAttackPatternComponent(): PreviousPlayedCard(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}

void UAttackPatternComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	// 공격 패턴 데이터가 비어 있으면 초기화하지 않고 함수 종료
	if (AttackPatternData.IsNull()) return;

	// 데이터 테이블에서 공격 패턴을 찾음
	FCardPattern* CardPatternData = AttackPatternData.DataTable->FindRow<FCardPattern>(AttackPatternData.RowName, TEXT(""));
	if (!CardPatternData) return;

	// 공격 패턴 데이터를 초기화
	AttackPattern = CardPatternData->CardPattern;
	RepeatFrom = CardPatternData->RepeatFrom;

	// 소유자가 파괴될 때 CleanUpOnOwnerDeath 함수가 호출되도록 바인딩
	GetOwner()->OnDestroyed.AddDynamic(this, &UAttackPatternComponent::CleanUpOnOwnerDeath);

	// 소유자가 카드 타겟 인터페이스를 구현하고 있는지 확인 후, 상태를 추가
	if (GetOwner()->Implements<UInterface_CardTarget>())
	{
		IInterface_CardTarget::Execute_AddToStatus(GetOwner(), UStatus_AttackPattern::StaticClass(), 1, false, nullptr);
	}
}

ACardBase* UAttackPatternComponent::GetNextCard()
{
	// 패턴 인덱스를 증가시킴
	PatternIndex++;

	// 패턴 인덱스가 공격 패턴 배열을 초과하면 반복 시작 지점으로 리셋
	if (PatternIndex >= AttackPattern.Num())
	{
		PatternIndex = RepeatFrom;
	}

	// 현재 인덱스에서 무작위로 카드 선택
	return ChooseRandomCardOption(AttackPattern[PatternIndex]);
}

ACardBase* UAttackPatternComponent::ChooseRandomCardOption(const FCardOptions& InCardOptions)
{
	// 카드 옵션의 가중치 총합 계산
	float TotalWeight = 0.0f;
	for (const FCardOption& CardOption : InCardOptions.CardOptions)
	{
		TotalWeight += CardOption.Weight;
	}

	// 0부터 가중치 총합 사이의 랜덤 값을 선택
	float RandomWeight = FMath::FRandRange(0.0f, TotalWeight);

	// 선택된 가중치에 해당하는 카드를 찾기 위해 현재까지의 가중치를 누적
	float AccumulatedWeight = 0.0f;
	for (int32 Index = 0; Index < InCardOptions.CardOptions.Num(); ++Index)
	{
		const FCardOption& CardOption = InCardOptions.CardOptions[Index];
		AccumulatedWeight += CardOption.Weight;

		// 누적 가중치가 선택된 랜덤 가중치보다 크거나 같으면 해당 카드를 선택
		if (AccumulatedWeight >= RandomWeight)
		{
			PreviousPlayedCard = GetCardOrNextIfMaxRepeated(CardOption.Card, CardOption.MaxRepeat, Index, InCardOptions);
			return PreviousPlayedCard;
		}
	}

	return nullptr; // 조건에 맞는 카드가 없을 경우 null 반환 (정상적으로는 이 경우 발생하지 않음)
}

ACardBase* UAttackPatternComponent::GetCardOrNextIfMaxRepeated(const FDataTableRowHandle& InCardData, int32 InMaxRepeat, int InCardOptionIndex, const FCardOptions& InCardOptions)
{
	// 카드 인스턴스를 가져옴 (새로 생성되거나 기존에 존재하는 카드)
	ACardBase* CurrentCard = GetCardLazy(InCardData);

	// 이전에 플레이한 카드가 다른 경우, 반복 횟수를 초기화하고 현재 카드를 반환
	if (PreviousPlayedCard != CurrentCard)
	{
		CurrentRepetitions = 1;
		return CurrentCard;
	}

	// 현재 카드가 최대 반복 횟수를 초과하지 않았거나 공격 패턴이 비어있는 경우, 반복 횟수를 증가시키고 반환
	if (CurrentRepetitions < InMaxRepeat || AttackPattern.Num() == 0)
	{
		CurrentRepetitions++;
		return CurrentCard;
	}

	// 최대 반복 횟수를 초과한 경우, 다음 카드 옵션을 선택하여 반환
	int32 NextCardOptionIndex = (InCardOptionIndex + 1) % InCardOptions.CardOptions.Num(); // 다음 카드 인덱스 계산
	const FDataTableRowHandle& NextCardDataHandle = InCardOptions.CardOptions[NextCardOptionIndex].Card; // 다음 카드 데이터 핸들 가져옴

	CurrentRepetitions++; // 반복 횟수 증가
	return GetCardLazy(NextCardDataHandle); // 다음 카드 인스턴스를 가져옴
}

ACardBase* UAttackPatternComponent::GetCardLazy(const FDataTableRowHandle& InCardData)
{
	FString TableName = GetNameSafe(InCardData.DataTable); // DataTable 이름을 가져옴
	FString RowName = InCardData.RowName.ToString(); // RowName을 문자열로 변환
	FString UniqueKey = FString::Printf(TEXT("%s,%s"), *TableName, *RowName); // TableName과 RowName을 결합하여 고유 키 생성

	// 만약 카드가 이미 존재한다면, 상태 초기화 후 반환
	if (Cards.Contains(UniqueKey))
	{
		Cards[UniqueKey]->ResetInHandModifications(); // 카드 상태 초기화
		return Cards[UniqueKey]; // 기존 카드 반환
	}

	// 존재하지 않으면 새로운 카드를 생성
	FTransform DefaultTransform = FTransform::Identity;

	// 카드를 지연 생성
	ACardBase* NewCard = GetWorld()->SpawnActorDeferred<ACardBase>(
		ACardBase::StaticClass(),
		DefaultTransform,
		GetOwner(),
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
		ESpawnActorScaleMethod::OverrideRootScale
	);


	// 생성된 카드에 데이터 테이블에서 찾은 카드 데이터를 설정
	NewCard->CardDataDeck = *InCardData.DataTable->FindRow<FCard>(InCardData.RowName, TEXT(""));

	// 카드 생성을 완료하고 맵에 추가
	NewCard->FinishSpawning(DefaultTransform);
	Cards.Add(UniqueKey, NewCard); // 생성된 카드를 맵에 저장

	return NewCard; // 새로 생성된 카드 반환
}

void UAttackPatternComponent::CleanUpOnOwnerDeath(AActor* DestroyedActor)
{
	// 모든 카드 인스턴스를 순회하며 파괴
	for (const TPair<FString, ACardBase*>& CardEntry : Cards)
	{
		if (CardEntry.Value) // 카드가 유효한지 확인
		{
			CardEntry.Value->Destroy(); // 유효한 카드 파괴
		}
	}
}
