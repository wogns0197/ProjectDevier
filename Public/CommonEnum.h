#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EOverlapObjectType
{
	NONE = -1,

	Interactive,

	COUNT,
};

UENUM()
enum class EInteractiveType
{
	NONE = -1,

	Picking,

	COUNT,
};

UENUM()
enum class EAnimNotifyType
{
	Pickup,
	Punch,
};

class PROJECTD_API CommonEnum
{
public:
	CommonEnum();
	~CommonEnum();
};
