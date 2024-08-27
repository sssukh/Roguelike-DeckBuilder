#pragma once

#include "CoreMinimal.h"



#define ECC_PathTrace ECC_GameTraceChannel1
#define ECC_RangeTrace ECC_GameTraceChannel2
#define ECC_CoverTrace ECC_GameTraceChannel3
#define ECC_WallTrace ECC_GameTraceChannel4


#define ETQ_PathTrace UEngineTypes::ConvertToTraceType(ECC_PathTrace)