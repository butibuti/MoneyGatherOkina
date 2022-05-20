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

	gameObject.lock()->GetGameComponent<SphereExclusion>()->SetWeight(100.0f);
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
	m_vlp_enemy = gameObject.lock()->GetGameComponent<Enemy>();
	m_vlp_enemy->CreatePocket(3);
	m_vlp_enemy->SetVibrationCapacity(100.0f);
	m_vlp_enemy->SetVibrationResistance(1.0f);
	m_vlp_enemy->SetExplosionScale(2.0f);
	m_vlp_enemy->SetWeight(100.0f);
}
