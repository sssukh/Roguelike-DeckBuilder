

#include "Core/CosEnumStruct.h"

#include "StatusSystem/StatusComponent.h"

bool FStatusData::operator==(const FStatusData& Other) const
{
	return StatusClass == Other.StatusClass;
}
