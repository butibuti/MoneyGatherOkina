#include "stdafx_u.h"
#include "Enemy_Boss.h"
#include "Enemy.h"
#include "SeparateDrawObject.h"
#include "SphereExclusion.h"

void ButiEngine::Enemy_Boss::OnUpdate()
{
}

void ButiEngine::Enemy_Boss::OnSet()
{
}

void ButiEngine::Enemy_Boss::OnShowUI()
{
}

void ButiEngine::Enemy_Boss::Start()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->CreateDrawObject("Boss");
	SetEnemyParameter();

	gameObject.lock()->GetGameComponent<SphereExclusion>()->SetWeight(100.0f);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Enemy_Boss::Clone()
{
	return ObjectFactory::Create<Enemy_Boss>();
}

void ButiEngine::Enemy_Boss::Dead()
{
}

void ButiEngine::Enemy_Boss::SetEnemyParameter()
{
	m_vwp_enemyComponent = gameObject.lock()->GetGameComponent<Enemy>();
	m_vwp_enemyComponent.lock()->CreatePocket(16);
	m_vwp_enemyComponent.lock()->SetVibrationCapacity(10000.0f);
	m_vwp_enemyComponent.lock()->SetVibrationResistance(5.0f);
	m_vwp_enemyComponent.lock()->SetExplosionScale(8.0f);
	m_vwp_enemyComponent.lock()->SetWeight(1000.0f);
}
