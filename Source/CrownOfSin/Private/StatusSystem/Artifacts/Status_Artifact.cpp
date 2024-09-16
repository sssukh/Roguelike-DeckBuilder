#include "StatusSystem/Artifacts/Status_Artifact.h"

#include "CardSystem/CardPlayer.h"
#include "ActionSystem/Action_ModifyStatus.h"
#include "Core/MinionBase.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "Utilities/CosGameplayTags.h"


// Sets default values for this component's properties
UStatus_Artifact::UStatus_Artifact()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UStatus_Artifact::BeginPlay()
{
	// 소유자가 ACardPlayer 타입일 경우
	if (ACardPlayer* OwnerCardPlayer = Cast<ACardPlayer>(GetOwner()))
	{
		// ACardPlayer 소유자는 특별한 처리 없이 기본 로직을 실행
		Super::BeginPlay();
		return;
	}

	// 소유자가 AMinionBase 타입일 경우
	if (AMinionBase* OwnerMinion = Cast<AMinionBase>(GetOwner()))
	{
		// 미니언의 게임플레이 태그를 가져옴
		const FGameplayTagContainer& MinionGameplayTags = OwnerMinion->GetGameplayTags();

		// 미니언이 'Target_Hero' 태그를 가지고 있을 경우
		if (MinionGameplayTags.HasTagExact(CosGameTags::Target_Hero))
		{
			// 카드 게임 인스턴스를 가져옴
			UGameInstance* CardGameInstance = UFunctionLibrary_Singletons::GetCardGameInstance(this);
			if (!CardGameInstance) return; // 인스턴스가 없을 경우 함수 종료

			// 인스턴스에 아티팩트가 있는지 확인
			bool bHasArtifact = IInterface_CardGameInstance::Execute_CheckIfInstanceHasArtifact(CardGameInstance, GetClass());
			if (!bHasArtifact)
			{
				// 플레이어 객체를 다시 가져옴
				ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
				if (!CardPlayer) return;

				// 상태를 플레이어에 추가
				IInterface_CardTarget::Execute_AddToStatus(CardPlayer, GetClass(), StatusValue, false, nullptr);

				// 컴포넌트를 파괴하여 더 이상 사용되지 않도록 함
				DestroyComponent();
				return;
			}
		}
	}

	// 기본 BeginPlay 로직을 실행
	Super::BeginPlay();
}

int32 UStatus_Artifact::AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, bool bRefreshAppearance, UObject* InPayload)
{
	if (ACardPlayer* OwnerCardPlayer = Cast<ACardPlayer>(GetOwner()))
	{
		if (OwnerCardPlayer != InPayload)
		{
			UGameInstance* CardGameInstance = UFunctionLibrary_Singletons::GetCardGameInstance(this);
			IInterface_CardGameInstance::Execute_AddArtifactToInstance(CardGameInstance, FStatusData(GetClass(), InAmount, GameplayTags));
		}
	}

	return Super::AddStatusValue(InAmount, bShowSplashNumber, bShowSplashIcon, bRefreshAppearance, InPayload);
}

bool UStatus_Artifact::Interact_Implementation(const FGameplayTagContainer& Tags)
{
	if (Tags.HasTagExact(CosGameTags::StatusBar_RewardBar))
	{
		ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
		if (!CardPlayer) return false;

		IInterface_CardTarget::Execute_AddToStatus(CardPlayer, GetClass(), StatusValue, true, nullptr);

		FTransform SpawnTransform = FTransform::Identity;

		if (AAction_ModifyStatus* NewActionModifyStatus = GetWorld()->SpawnActorDeferred<AAction_ModifyStatus>(AAction_ModifyStatus::StaticClass(),
		                                                                                                       SpawnTransform,
		                                                                                                       nullptr, nullptr,
		                                                                                                       ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
		{
			NewActionModifyStatus->NewValue = 0;
			NewActionModifyStatus->bShowSplashIcon = false;
			NewActionModifyStatus->bShowSplashNumber = false;
			NewActionModifyStatus->bRefreshAppearance = true;
			NewActionModifyStatus->TextOverride = FText::FromString(TEXT("HideReward"));
			NewActionModifyStatus->bCanBeZero = false;
			NewActionModifyStatus->EndDelay = -1.0f;
			NewActionModifyStatus->FinishSpawning(SpawnTransform);
		}

		return true;
	}

	return true;
}
