// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_CardAction.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInterface_CardAction : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CROWNOFSIN_API IInterface_CardAction
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/*
	 * 하위 청사진을 재정의하는 데 사용됩니다. 액션이 생성되면 즉시 설정 코드를 실행합니다.
	 * 액션이 대기열에 추가될 때와 실행될 때 사이에 소멸될 수 있는 액터의 변수에 액세스하려는 경우 유용합니다.
	 * 그렇다면 여기에서 액세스하여 액션을 플레이할 때 사용할 변수에 저장할 수 있습니다.
	 */	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Action")
	void SetUpAction();

	/*
	 * 하위 블루프린트에서 재정의되었습니다.
	 * 여기에서 발생하는 작업을 표시하려는 모든 동작을 실행합니다.
	 * PlayAction은 일반적으로 게임플레이 관련 코드를 포함하지 않아야 하며 플레이어에게 사물을 시각화하는 데에만 사용되어야 합니다.
	 */	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Action")
	void PlayAction();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Action")
	void EndAction();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Action")
	void AnimateAction(UObject* CallingActionManager);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Action")
	bool AttemptDestroyAction();
};
