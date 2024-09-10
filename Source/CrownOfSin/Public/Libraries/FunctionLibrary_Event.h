#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FunctionLibrary_Event.generated.h"

/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UFunctionLibrary_Event : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="COS|Function Library|Event")
	static void BindEventToGlobalDispatcherHub(UObject* EventHolder, FGameplayTag EventTag);

	UFUNCTION(BlueprintCallable, Category="COS|Function Library|Event")
	static void UnBindEventFromGlobalDispatcherHub(UObject* EventHolder, FGameplayTag EventTag);

	UFUNCTION(BlueprintCallable, Category="COS|Function Library|Event")
	static bool CallEventInGlobalDispatcherHub(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject = nullptr);

	UFUNCTION(BlueprintCallable, Category="COS|Function Library|Event")
	static bool QueueEventInGlobalDispatcherHub(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject = nullptr,
	                                            float EndDelay = -1.0f, UObject* Payload = nullptr, FGameplayTagContainer CallTags = FGameplayTagContainer());

	UFUNCTION(BlueprintCallable, Category="COS|Function Library|Event")
	static bool CallMultipleEventsInGlobalDispatcherHub(FGameplayTagContainer EventTags, UObject* CallingObject, UObject* CallSpecificObject);
};
