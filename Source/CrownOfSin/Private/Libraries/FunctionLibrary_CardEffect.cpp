#include "Libraries/FunctionLibrary_CardEffect.h"

#include "Interfaces/Interface_CardTarget.h"
#include "StatusSystem/StatusComponent.h"

int32 UFunctionLibrary_CardEffect::AddToStatusCappedToMax(AActor* TargetActor, TSubclassOf<UStatusComponent> StatusClass, int32 Amount, bool bShowSplash, UObject* Payload)
{
	if (TargetActor->Implements<UInterface_CardTarget>())
	{
		return 0;
	}

	UStatusComponent* FoundStatusComponent = Cast<UStatusComponent>(TargetActor->GetComponentByClass(StatusClass));
	if (IsValid(FoundStatusComponent))
	{
		if (FoundStatusComponent->bMaxAble)
		{
			int32 NewStatusValue = Amount + FoundStatusComponent->StatusValue;
			int32 CalculateStatusValue = FMath::Min(FoundStatusComponent->MaxValue, NewStatusValue);
			int32 NewAmount = CalculateStatusValue - FoundStatusComponent->StatusValue;
			return IInterface_CardTarget::Execute_AddToStatus(TargetActor, StatusClass, NewAmount, bShowSplash, Payload);
		}
	}


	return IInterface_CardTarget::Execute_AddToStatus(TargetActor, StatusClass, Amount, bShowSplash, Payload);
}
