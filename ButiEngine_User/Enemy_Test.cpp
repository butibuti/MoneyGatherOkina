#include "stdafx_u.h"
#include "Enemy_Test.h"
#include "Enemy.h"

void ButiEngine::Enemy_Test::OnUpdate()
{
}

void ButiEngine::Enemy_Test::OnSet()
{
}

void ButiEngine::Enemy_Test::OnShowUI()
{
}

void ButiEngine::Enemy_Test::Start()
{
	auto enemyComponent = gameObject.lock()->GetGameComponent<Enemy>();
	enemyComponent->CreatePocket(8);
	enemyComponent->SetNearBorder(6.5f);
	enemyComponent->SetVibrationCapacity(1000000.0f);
	enemyComponent->SetVibrationResistance(3.0f);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Enemy_Test::Clone()
{
	return ObjectFactory::Create<Enemy_Test>();
}
