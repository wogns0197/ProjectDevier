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
enum class EInteractiveType // 우선순위 순서 유지 필요
{
	NONE = -1,

	Picking,
	Trembling, // 나무 흔들기

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
