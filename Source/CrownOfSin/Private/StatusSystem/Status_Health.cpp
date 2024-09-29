#include "StatusSystem/Status_Health.h"

#include "ActionSystem/ActionManagerSubsystem.h"
#include "ActionSystem/Action_SimpleAnim.h"
#include "Core/DispatcherHubComponent.h"
#include "Core/MinionBase.h"
#include "Libraries/AssetPath.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


// Sets default values for this component's properties
UStatus_Health::UStatus_Health()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	static ConstructorHelpers::FObjectFinder<UTexture2D> T_Heart(*AssetPath::Texture::T_Heart);
	if (T_Heart.Succeeded())
	{
		Icon = T_Heart.Object;
	}

	StatusValue = 1;
	SlotType = EStatusSlot::Bar;
	Tint = FLinearColor::Red;
	Priority = 20.0f;
	bMaxAble = true;
}

int32 UStatus_Health::AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, bool bRefreshAppearance, UObject* InPayload)
{
	// 체력이 증가하는 경우, 부모 클래스의 AddStatusValue를 호출하고 종료
	if (InAmount > 0)
	{
		return Super::AddStatusValue(InAmount, bShowSplashNumber, bShowSplashIcon, bRefreshAppearance, InPayload);
	}

	// 체력 감소를 처리
	IncomingStatusChange = InAmount;

	// DispatcherHub 유효성 검사
	UDispatcherHubComponent* OwnersDispatcherHub = nullptr;
	if (GetOwnersDispatcherHub(OwnersDispatcherHub))
	{
		// PreTakeDamage 이벤트 호출
		OwnersDispatcherHub->CallEvent(CosGameTags::Event_PreTakeDamage, this);
	}

	// 체력 감소 처리 및 부모 클래스의 AddStatusValue 호출
	bool bHealthDepleted = Super::AddStatusValue(IncomingStatusChange, bShowSplashNumber, bShowSplashIcon, bRefreshAppearance, InPayload) <= 0;

	// 소유자 액터 가져오기
	AMinionBase* OwnerMinion = Cast<AMinionBase>(GetOwner());
	if (!OwnerMinion)
	{
		COS_LOG_SCREEN_ERROR(TEXT("OwnerMinion을 찾을 수 없습니다."));
		return StatusValue;
	}

	// 체력이 0 이하로 떨어진 경우, 캐릭터를 죽음 상태로 표시
	if (bHealthDepleted)
	{
		OwnerMinion->bDead = true;
	}

	// 애니메이션 실행: 액션을 큐에 추가
	UActionManagerSubsystem* ActionManagerSubsystem = GetWorld()->GetSubsystem<UActionManagerSubsystem>();
	ActionManagerSubsystem->CreateAndQueueAction<AAction_SimpleAnim>([&](AAction_SimpleAnim* Action_SimpleAnim)
	{
		Action_SimpleAnim->Puppet = OwnerMinion->Puppet;
		// 체력 감소에 따른 애니메이션 선택
		Action_SimpleAnim->Animation = bHealthDepleted ? CosGameTags::Anim_Die : (IncomingStatusChange != 0 ? CosGameTags::Anim_Flinch : CosGameTags::Anim_Block);
		Action_SimpleAnim->EndDelay = -1.0f;
	});

	// PostTakeDamage 이벤트 호출
	if (InAmount < 0)
	{
		OwnersDispatcherHub->CallEvent(CosGameTags::Event_PostTakeDamage, this);

		// 체력이 0 이하일 경우, 캐릭터를 게임에서 제거
		if (StatusValue <= 0)
		{
			OwnerMinion->RemoveFromGame();
		}
	}

	return StatusValue;


	// if (InAmount > 0)
	// {
	// 	return Super::AddStatusValue(InAmount, bShowSplashNumber, bShowSplashIcon, bRefreshAppearance, InPayload);
	// }
	//
	// IncomingStatusChange = InAmount;
	//
	// UDispatcherHubComponent* OwnersDispatcherHub = nullptr;
	// GetOwnersDispatcherHub(OwnersDispatcherHub);
	//
	// OwnersDispatcherHub->CallEvent(CosGameTags::Event_PreTakeDamage, this);
	//
	// bool ParentResultLessEqualThanZero = Super::AddStatusValue(IncomingStatusChange, bShowSplashNumber, bShowSplashIcon, bRefreshAppearance, InPayload) <= 0;
	//
	// AMinionBase* OwnerMinion = Cast<AMinionBase>(GetOwner());
	//
	// if (ParentResultLessEqualThanZero)
	// {
	// 	OwnerMinion->bDead = true;
	// }
	//
	// UActionManagerSubsystem* ActionManagerSubsystem = GetWorld()->GetSubsystem<UActionManagerSubsystem>();
	// ActionManagerSubsystem->CreateAndQueueAction<AAction_SimpleAnim>([&](AAction_SimpleAnim* Action_SimpleAnim)
	// {
	// 	Action_SimpleAnim->Puppet = OwnerMinion->Puppet;
	//
	// 	FGameplayTag InAnimTag;
	//
	// 	if (ParentResultLessEqualThanZero)
	// 	{
	// 		InAnimTag = CosGameTags::Anim_Die;
	// 	}
	// 	else
	// 	{
	// 		if (IncomingStatusChange != 0)
	// 		{
	// 			InAnimTag = CosGameTags::Anim_Flinch;
	// 		}
	// 		else
	// 		{
	// 			InAnimTag = CosGameTags::Anim_Block;
	// 		}
	// 	}
	//
	// 	Action_SimpleAnim->Animation = InAnimTag;
	// });
	//
	// if (InAmount < 0)
	// {
	// 	OwnersDispatcherHub->CallEvent(CosGameTags::Event_PostTakeDamage, this);
	//
	// 	if (StatusValue <= 0)
	// 	{
	// 		OwnerMinion->RemoveFromGame();
	// 	}
	// }
	//
	// return StatusValue;
}
