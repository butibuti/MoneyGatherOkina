#include "stdafx_u.h"
#include "Enemy_Kiba.h"
#include "Enemy.h"
#include "Loiter.h"
#include "SeparateDrawObject.h"

void ButiEngine::Enemy_Kiba::OnUpdate()
{
}

void ButiEngine::Enemy_Kiba::OnSet()
{
}

void ButiEngine::Enemy_Kiba::OnShowUI()
{
}

void ButiEngine::Enemy_Kiba::Start()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->CreateDrawObject("Kiba");

	auto enemyComponent = gameObject.lock()->GetGameComponent<Enemy>();
	enemyComponent->CreatePocket(8);
	enemyComponent->SetNearBorder(gameObject.lock()->transform->GetLocalScale().x * 0.5f + 1.0f);
	enemyComponent->SetVibrationCapacity(1000.0f);
	enemyComponent->SetVibrationResistance(3.0f);

	auto loiterComponent = gameObject.lock()->GetGameComponent<Loiter>();
	loiterComponent->SetMoveRange(30.0f);
	loiterComponent->SetMaxMoveSpeed(0.1f);
	loiterComponent->SetWaitFrame(60);
	loiterComponent->SetAccelFrame(30);
	loiterComponent->SetBrakeFrame(30);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Enemy_Kiba::Clone()
{
	return ObjectFactory::Create<Enemy_Kiba>();
}
