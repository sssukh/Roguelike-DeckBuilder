#include "Utilities/CosGameplayTags.h"


namespace CosGameTags
{
	UE_DEFINE_GAMEPLAY_TAG(Target, TEXT("Target"))
	UE_DEFINE_GAMEPLAY_TAG(Target_Ally, TEXT("Target.Ally"))
	UE_DEFINE_GAMEPLAY_TAG(Target_Hero, TEXT("Target.Hero"))
	UE_DEFINE_GAMEPLAY_TAG(Target_Minion, TEXT("Target.Minion"))
	UE_DEFINE_GAMEPLAY_TAG(Target_Enemy, TEXT("Target.Enemy"))
	
	UE_DEFINE_GAMEPLAY_TAG(Event_CloseRewardScreen, TEXT("Event.CloseRewardScreen"))
	UE_DEFINE_GAMEPLAY_TAG(Event_Debug_DispathcerHubUpdate, TEXT("Event.Debug.DispathcerHubUpdate"))
}
