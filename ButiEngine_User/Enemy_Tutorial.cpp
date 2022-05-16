#include "stdafx_u.h"
#include "Enemy_Tutorial.h"
#include "Enemy.h"
#include "SphereExclusion.h"
#include "SeparateDrawObject.h"

void ButiEngine::Enemy_Tutorial::OnUpdate()
{
}

void ButiEngine::Enemy_Tutorial::OnSet()
{
}

void ButiEngine::Enemy_Tutorial::OnRemove()
{
}

void ButiEngine::Enemy_Tutorial::OnShowUI()
{
}

void ButiEngine::Enemy_Tutorial::Start()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->CreateDrawObject("Tutorial");

	SetEnemyParameter();

	gameObject.lock()->GetGameComponent<SphereExclusion>()->SetMass(1000.0f);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Enemy_Tutorial::Clone()
{
	return ObjectFactory::Create<Enemy_Tutorial>();
}

void ButiEngine::Enemy_Tutorial::Dead()
{
}

void ButiEngine::Enemy_Tutorial::SetEnemyParameter()
{
	auto enemy = gameObject.lock()->GetGameComponent<Enemy>();
	enemy->CreatePocket(3);
	enemy->SetNearBorder(gameObject.lock()->transform->GetLocalScale().x * 0.5f + 1.0f);
	enemy->SetVibrationCapacity(100.0f);
	enemy->SetVibrationResistance(1.0f);
	enemy->SetExplosionScale(2.0f);
}
