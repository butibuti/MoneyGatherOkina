#include "stdafx_u.h"
#include "Enemy_Flie.h"
#include "Enemy.h"
#include "Loiter.h"
#include "SeparateDrawObject.h"

void ButiEngine::Enemy_Flie::OnUpdate()
{
}

void ButiEngine::Enemy_Flie::OnSet()
{
}

void ButiEngine::Enemy_Flie::OnShowUI()
{
}

void ButiEngine::Enemy_Flie::Start()
{

	auto enemyComponent = gameObject.lock()->GetGameComponent<Enemy>();
	enemyComponent->CreatePocket(3);
	enemyComponent->SetNearBorder(gameObject.lock()->transform->GetLocalScale().x * 0.5f + 1.0f);
	enemyComponent->SetVibrationCapacity(100.0f);
	enemyComponent->SetVibrationResistance(3.0f);

	auto loiterComponent = gameObject.lock()->GetGameComponent<Loiter>();
	loiterComponent->SetMoveRange(20.0f);
	loiterComponent->SetMaxMoveSpeed(0.2f);
	loiterComponent->SetWaitFrame(60);
	loiterComponent->SetAccelFrame(30);
	loiterComponent->SetBrakeFrame(30);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Enemy_Flie::Clone()
{
	return ObjectFactory::Create<Enemy_Flie>();
}
