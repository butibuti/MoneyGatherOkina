#include "stdafx_u.h"
#include "Enemy_Fly.h"
#include "Enemy.h"
#include "Loiter.h"
#include "SeparateDrawObject.h"
#include "SphereExclusion.h"
#include "ShakeComponent.h"

void ButiEngine::Enemy_Fly::OnUpdate()
{
}

void ButiEngine::Enemy_Fly::OnSet()
{
}

void ButiEngine::Enemy_Fly::OnRemove()
{
}

void ButiEngine::Enemy_Fly::OnShowUI()
{
}

void ButiEngine::Enemy_Fly::Start()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->CreateDrawObject("Fly");

	SetEnemyParameter();
	SetLoiterParameter();
	SetLookAtParameter();

	gameObject.lock()->GetGameComponent<SphereExclusion>()->SetWeight(100.0f);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Enemy_Fly::Clone()
{
	return ObjectFactory::Create<Enemy_Fly>();
}

void ButiEngine::Enemy_Fly::Dead()
{
	if (m_vlp_enemy)
	{
		m_vlp_enemy->Explosion();
	}
}

void ButiEngine::Enemy_Fly::SetEnemyParameter()
{
	m_vlp_enemy = gameObject.lock()->GetGameComponent<Enemy>();
	m_vlp_enemy->CreatePocket(3);
	m_vlp_enemy->SetVibrationCapacity(10.0f);
	m_vlp_enemy->SetVibrationResistance(0.0f);
	m_vlp_enemy->SetExplosionScale(2.0f);
	m_vlp_enemy->SetWeight(100.0f);
}

void ButiEngine::Enemy_Fly::SetLoiterParameter()
{
	auto loiterComponent = gameObject.lock()->GetGameComponent<Loiter>();
	loiterComponent->SetMoveRange(2.0f);
	loiterComponent->SetMaxMoveSpeed(0.2f);
	loiterComponent->SetWaitFrame(60);
	loiterComponent->SetAccelFrame(30);
	loiterComponent->SetBrakeFrame(30);
}

void ButiEngine::Enemy_Fly::SetLookAtParameter()
{
	auto lookAt = gameObject.lock()->GetGameComponent<LookAtComponent>();
	lookAt->SetLookTarget(gameObject.lock()->transform->Clone());
	lookAt->GetLookTarget()->Translate(gameObject.lock()->transform->GetFront());
	lookAt->SetSpeed(0.1f);
}
