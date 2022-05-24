#include "stdafx_u.h"
#include "Crystal.h"
#include "Enemy.h"
#include "SeparateDrawObject.h"
#include "SphereExclusion.h"

std::int32_t ButiEngine::Crystal::m_pocketCount = 6;
float ButiEngine::Crystal::m_createPocketRadius = 2.5f;

void ButiEngine::Crystal::OnUpdate()
{
	//ƒ_ƒ[ƒW”»’èŽæ‚èÁ‚µ
	if (m_vlp_removeTagTimer->Update())
	{
		gameObject.lock()->RemoveGameObjectTag(GameObjectTag("PlayerDamageArea"));
		m_vlp_removeTagTimer->Stop();
	}

	switch (m_phase)
	{
	case ButiEngine::CrystalPhase::Appear:
		Appear();
		break;
	case ButiEngine::CrystalPhase::Wait:
		Wait();
		break;
	case ButiEngine::CrystalPhase::Disappear:
		Disappeaer();
		break;
	default:
		break;
	}
}

void ButiEngine::Crystal::OnSet()
{
}

void ButiEngine::Crystal::OnRemove()
{
}

void ButiEngine::Crystal::OnShowUI()
{
}

void ButiEngine::Crystal::Start()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->CreateDrawObject("Crystal");

	SetEnemyParameter();
	SetPhaseParameter();

	gameObject.lock()->GetGameComponent<SphereExclusion>()->SetWeight(100.0f);

	m_vlp_removeTagTimer = ObjectFactory::Create<RelativeTimer>(5);
	m_vlp_removeTagTimer->Start();

	m_defaultScale = Vector3(5.0f, 5.0f, 5.0f);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Crystal::Clone()
{
	return ObjectFactory::Create<Crystal>();
}

void ButiEngine::Crystal::Dead()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->Dead();
	gameObject.lock()->SetIsRemove(true);
}

void ButiEngine::Crystal::Appear()
{
	float progress = m_vlp_appearTimer->GetPercent() * GameDevice::WorldSpeed;
	progress = min(progress, 1.0f);

	Vector3 scale = m_defaultScale * Easing::EaseOutElastic(progress);
	gameObject.lock()->transform->SetLocalScale(scale);

	if (m_vlp_appearTimer->Update())
	{
		m_vlp_appearTimer->Stop();
		WaitStart();
	}
}

void ButiEngine::Crystal::AppearStart()
{
	m_phase = CrystalPhase::Appear;
	m_vlp_appearTimer->Start();
}

void ButiEngine::Crystal::Wait()
{
	if (m_vlp_waitTimer->Update())
	{
		m_vlp_waitTimer->Stop();
		DisappeaerStart();
	}
}

void ButiEngine::Crystal::WaitStart()
{
	m_phase = CrystalPhase::Wait;
	m_vlp_waitTimer->Start();
}

void ButiEngine::Crystal::Disappeaer()
{
	float progress = m_vlp_disappearTimer->GetPercent() * GameDevice::WorldSpeed;
	progress = min(progress, 1.0f);

	Vector3 scale = m_defaultScale * Easing::EaseOutElastic(1.0f - progress);
	gameObject.lock()->transform->SetLocalScale(scale);

	if (m_vlp_disappearTimer->Update())
	{
		m_vlp_disappearTimer->Stop();
		gameObject.lock()->GetGameComponent<Enemy>()->Dead();
	}
}

void ButiEngine::Crystal::DisappeaerStart()
{
	m_phase = CrystalPhase::Disappear;

	//“–‚½‚è”»’èíœ
	auto collider = gameObject.lock()->GetGameComponent<Collision::ColliderComponent>();
	if (collider)
	{
		collider->SetIsRemove(true);
	}

	m_vlp_disappearTimer->Start();
}

void ButiEngine::Crystal::SetEnemyParameter()
{
	auto vlp_enemyComponent = gameObject.lock()->GetGameComponent<Enemy>();
	vlp_enemyComponent->CreatePocket(m_pocketCount, m_createPocketRadius);
	vlp_enemyComponent->SetVibrationCapacity(1000.0f);
	vlp_enemyComponent->SetVibrationResistance(1.0f);
	vlp_enemyComponent->SetExplosionScale(2.0f);
	vlp_enemyComponent->SetWeight(100.0f);
}

void ButiEngine::Crystal::SetPhaseParameter()
{
	m_vlp_appearTimer = ObjectFactory::Create<RelativeTimer>(30);
	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>(180);
	m_vlp_disappearTimer = ObjectFactory::Create<RelativeTimer>(10);

	AppearStart();
}
