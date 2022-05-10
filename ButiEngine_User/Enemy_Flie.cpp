#include "stdafx_u.h"
#include "Enemy_Flie.h"
#include "Enemy.h"
#include "Loiter.h"
#include "SeparateDrawObject.h"
#include "SphereExclusion.h"

void ButiEngine::Enemy_Flie::OnUpdate()
{
}

void ButiEngine::Enemy_Flie::OnSet()
{
}

void ButiEngine::Enemy_Flie::OnRemove()
{
	if (m_vlp_enemy)
	{
		m_vlp_enemy->Explosion();
	}
}

void ButiEngine::Enemy_Flie::OnShowUI()
{
}

void ButiEngine::Enemy_Flie::Start()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->CreateDrawObject("Flie");

	SetEnemyParameter();
	SetLoiterParameter();

	gameObject.lock()->GetGameComponent<SphereExclusion>()->SetMass(50.0f);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Enemy_Flie::Clone()
{
	return ObjectFactory::Create<Enemy_Flie>();
}

void ButiEngine::Enemy_Flie::SetEnemyParameter()
{
	m_vlp_enemy = gameObject.lock()->GetGameComponent<Enemy>();
	m_vlp_enemy->CreatePocket(3);
	m_vlp_enemy->SetNearBorder(gameObject.lock()->transform->GetLocalScale().x * 0.5f + 1.0f);
	m_vlp_enemy->SetVibrationCapacity(100.0f);
	m_vlp_enemy->SetVibrationResistance(3.0f);
	m_vlp_enemy->SetExplosionScale(2.0f);
}

void ButiEngine::Enemy_Flie::SetLoiterParameter()
{
	auto loiterComponent = gameObject.lock()->GetGameComponent<Loiter>();
	loiterComponent->SetMoveRange(20.0f);
	loiterComponent->SetMaxMoveSpeed(0.2f);
	loiterComponent->SetWaitFrame(60);
	loiterComponent->SetAccelFrame(30);
	loiterComponent->SetBrakeFrame(30);
}
