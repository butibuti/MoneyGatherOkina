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
	enemyComponent->SetVibrationCapacity(100.0f);
	enemyComponent->SetVibrationResistance(10.0f);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Enemy_Test::Clone()
{
	return ObjectFactory::Create<Enemy_Test>();
}
