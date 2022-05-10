#include "stdafx_u.h"
#include "VolcanoRock.h"
#include "ParticleGenerater.h"
#include "SeparateDrawObject.h"

void ButiEngine::VolcanoRock::OnUpdate()
{
	m_velocity.y -= m_gravity;
	Vector3 velocity = m_velocity;
	velocity.Normalize();
	velocity.x *= 0.075f;
	velocity.y *= 0.2f;
	velocity.z *= 0.075f;

	gameObject.lock()->transform->Translate(velocity);



	if (m_life > 0)
	{
		m_life--;
	}
	else
	{
		//ライフが消えたら爆発エフェクトを出して死ぬ
		//ここで爆発エフェクト発生
		//auto position = gameObject.lock()->transform->GetWorldPosition();
		//m_vwp_particleGenerater.lock()->ExplosionParticles(position);
		auto transform = gameObject.lock()->transform;
		auto deadEffect = GetManager().lock()->AddObjectFromCereal("SplashEffect");
		deadEffect.lock()->transform->SetLocalPosition(transform->GetLocalPosition());
		deadEffect.lock()->transform->SetLocalScale(transform->GetLocalScale() * 5.0f);
		gameObject.lock()->SetIsRemove(true); //死
	}
}

void ButiEngine::VolcanoRock::OnSet()
{
}

void ButiEngine::VolcanoRock::OnShowUI()
{
}

void ButiEngine::VolcanoRock::Start()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->CreateDrawObject("Rock");
	m_vwp_particleGenerater = GetManager().lock()->GetGameObject("ParticleController").lock()->GetGameComponent<ParticleGenerater>();
	m_gravity = 0.05f;
	m_life = 105;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::VolcanoRock::Clone()
{
	return ObjectFactory::Create<VolcanoRock>();
}

void ButiEngine::VolcanoRock::SetVelocity(const Vector3& arg_velocity)
{
	m_velocity = arg_velocity;
}
