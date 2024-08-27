#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Node.generated.h"

class ANode;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNodeSelected, ANode*, InNode);

UCLASS()
class CROWNOFSIN_API ANode : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*ToDo : 구현해야함*/
	void CrossOut()
	{
	}

	/*ToDo : 구현해야함*/
	void Enable()
	{
	}

	/*ToDo : 구현해야함*/
	void StopEnable()
	{
		
	}


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	bool bOrigin = false;

	UPROPERTY(BlueprintReadWrite, Category="Node")
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Node")
	TArray<ANode*> Connections;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Node|Delegate Event")
	FOnNodeSelected OnNodeSelected;
};
