#include "UI/UW_Layout_RLDB.h"


UUW_Layout_RLDB::UUW_Layout_RLDB(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UUW_Layout_RLDB::ShouldNodeMapBeBlocked()
{
	return true;
}
