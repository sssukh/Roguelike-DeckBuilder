
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Core/CosEnumStruct.h"
#include "StatusComponent.generated.h"


UCLASS(ClassGroup=("Cos"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*ToDo*/
	int32 AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, bool bRefreshAppearance, UObject* InPayload) { return 0; }

	/*ToDo 이거 인터페이슴 기존에 얘로 호출된거 다 인터페이스로 변경*/
	bool Interact(const FGameplayTagContainer& Tags) { return false; }

	/*ToDo*/
	FStatusAppearance MakeAppearanceStruct()
	{
		return {};
	};


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(BlueprintReadWrite, Category="Status Component")
	bool bCanBeZero = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Component", meta=(ExposeOnSpawn="true"))
	bool bShowImmediately;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Component", meta=(ExposeOnSpawn="true"))
	int32 StatusValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Component", meta=(ExposeOnSpawn="true"))
	FGameplayTag GameplayTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Component", meta=(ExposeOnSpawn="true"))
	UObject* OwnerUiRef;
};
