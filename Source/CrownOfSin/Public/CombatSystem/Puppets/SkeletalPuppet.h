#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interface_CardPuppet.h"
#include "SkeletalPuppet.generated.h"

class AMinionBase;
class UPuppetComponent;
class UBoxComponent;
class UWidgetComponent;

/*스켈레탈 메시로 미니언을 표시하고 플레이어가 해당 미니언을 타겟팅할 수 있도록 하는 Puppet 클래스입니다.
 *BP_Minion에서 처리해야 하는 게임플레이 코드가 아닌 시각화만을 위한 것입니다.*/
UCLASS()
class CROWNOFSIN_API ASkeletalPuppet : public AActor, public IInterface_CardPuppet
{
	GENERATED_BODY()

public:
	ASkeletalPuppet();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category="Skeletal Puppet Event")
	void SetRenderCustomDepthForPrimitives(const TArray<UPrimitiveComponent*>& PrimitiveComponents, bool bVisibility, int32 StencilValue);

	UFUNCTION(BlueprintCallable, Category="Skeletal Puppet Event")
	void SetRenderCustomDepthForEntirePuppet(bool bVisibility, int32 StencilValue);


	/*========================================================================================
	*	IInterface_CardPuppet
	=========================================================================================*/
public:
	/**
	 * 미니언을 초기화하고, 미니언의 데이터에 기반하여 스켈레탈 메시, 애니메이션, UI, 콜라이더 등을 설정하는 함수입니다.
	 * 게임 중 스폰된 경우 페이드 인 애니메이션과 함께 나타나도록 처리합니다.
	 */
	virtual void InitializePuppet_Implementation(AMinionBase* InMinion, bool bSpawnedDuringGameplay) override;

	virtual void AnimatePuppet_Implementation(FGameplayTag InAnimationTag) override;

	virtual void SignalAnimEvent_Implementation(FGameplayTag InAnimationTag, FName InNotify) override;

	virtual void SignalAnimEnd_Implementation(FGameplayTag InAnimationTag) override;

	virtual void MarkPuppet_Implementation() override;

	virtual void UnmarkPuppet_Implementation() override;

	virtual bool GetMinionFromPuppet_Implementation(AMinionBase*& OutMinion) override;

	virtual UObject* GetPuppetUI_Implementation(EStatusSlot InSlot) override;

	virtual FVector GetPuppetRelativeCenter_Implementation() override;


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skeletal Puppet|Component")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skeletal Puppet|Component")
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skeletal Puppet|Component")
	UWidgetComponent* BottomUIWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skeletal Puppet|Component")
	UWidgetComponent* TopUIWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skeletal Puppet|Component")
	UBoxComponent* MouseCollider;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Skeletal Puppet|Component")
	UPuppetComponent* PuppetComponent;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skeletal Puppet")
	float DisappearFadeDelay = 0.5f;

	UPROPERTY(BlueprintReadWrite, Category="Skeletal Puppet")
	AMinionBase* ParentMinion;

	UPROPERTY(BlueprintReadWrite, Category="Skeletal Puppet")
	TArray<UMaterialInstanceDynamic*> DynamicMeshMaterials;
};
