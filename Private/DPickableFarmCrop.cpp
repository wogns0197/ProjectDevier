#include "DPickableFarmCrop.h"
#include "CommonEnum.h"

// 위에서 떨어지는 열매는 중력 피직스는 받되, Collision이 캐릭터는 Overlap 되도록 해야함.
// 따라서 BP 콜리전 프리셋을 아래와 같이 설정해줘야한다 ( 코드로 하려면 떨어지는 객체 구조로 상속받게 바꿔야 하는데 이건 나중에 )
// Custom
// Collision Enabled
// WorldDynamic
// Camera, Pawn, PhysicsBody, Vehicle = Overlap


void ADPickableFarmCrop::BeginPlay()
{
	Super::BeginPlay();
	InteractiveType = EInteractiveType::Picking;
}
