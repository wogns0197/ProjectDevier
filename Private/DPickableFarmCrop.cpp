#include "DPickableFarmCrop.h"
#include "CommonEnum.h"

// ������ �������� ���Ŵ� �߷� �������� �޵�, Collision�� ĳ���ʹ� Overlap �ǵ��� �ؾ���.
// ���� BP �ݸ��� �������� �Ʒ��� ���� ����������Ѵ� ( �ڵ�� �Ϸ��� �������� ��ü ������ ��ӹް� �ٲ�� �ϴµ� �̰� ���߿� )
// Custom
// Collision Enabled
// WorldDynamic
// Camera, Pawn, PhysicsBody, Vehicle = Overlap


void ADPickableFarmCrop::BeginPlay()
{
	Super::BeginPlay();
	InteractiveType = EInteractiveType::Picking;
}
