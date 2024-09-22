#pragma once

#include "CoreMinimal.h"
#include "Core/CosEnumStruct.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interface_CardPuppet.h"
#include "PaperPuppet.generated.h"

class UPaperFlipbook;
class UPaperFlipbookComponent;
class UPuppetComponent;
class UBoxComponent;
class UWidgetComponent;

class AMinionBase;

UCLASS()
class CROWNOFSIN_API APaperPuppet : public AActor, public IInterface_CardPuppet
{
	GENERATED_BODY()

public:
	APaperPuppet();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category="Paper Puppet Event")
	void SetRenderCustomDepthForPrimitives(const TArray<UPrimitiveComponent*>& PrimitiveComponents, bool bVisibility, int32 StencilValue);

	UFUNCTION(BlueprintCallable, Category="Paper Puppet Event")
	void SetRenderCustomDepthForEntirePuppet(bool bVisibility, int32 StencilValue);

protected:
	UFUNCTION()
	void HandleAnimatePuppet();

public:
	/*========================================================================================
	*	IInterface_CardPuppet
	=========================================================================================*/
	virtual void InitializePuppet_Implementation(AMinionBase* InMinion, bool bSpawnedDuringGameplay) override;

	virtual void AnimatePuppet_Implementation(FGameplayTag InAnimationTag) override;

	virtual void MarkPuppet_Implementation() override;

	virtual void UnmarkPuppet_Implementation() override;

	virtual void AppearPuppet_Implementation(float InSpeedProp) override;

	virtual void DisappearPuppet_Implementation(float InSpeedProp) override;

	virtual bool GetMinionFromPuppet_Implementation(AMinionBase*& OutMinion) override;

	virtual UObject* GetPuppetUI_Implementation(EStatusSlot InSlot) override;

	virtual FVector GetPuppetRelativeCenter_Implementation() override;


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Papper Puppet|Component")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Papper Puppet|Component")
	USceneComponent* PaperScene;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Papper Puppet|Component")
	UPaperFlipbookComponent* PuppetFlipbook;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Papper Puppet|Component")
	UWidgetComponent* MinionUI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Papper Puppet|Component")
	UBoxComponent* MouseCollider;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Papper Puppet|Component")
	UPuppetComponent* PuppetComponent;

public:
	UPROPERTY(BlueprintReadWrite, Category="Papper Puppet")
	AMinionBase* ParentMinion;

	UPROPERTY(BlueprintReadWrite, Category="Papper Puppet")
	FGameplayTag CurrentAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Papper Puppet")
	TMap<FGameplayTag, UPaperFlipbook*> AnimFlipbooks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Papper Puppet")
	TMap<UPaperFlipbook*, int32> FlipbookHitFrames;
};
