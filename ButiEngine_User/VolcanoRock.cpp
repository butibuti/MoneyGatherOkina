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
	gameObject.lock()->transform->RollLocalRotationX_Degrees(m_rotateSpeed.x);
	gameObject.lock()->transform->RollLocalRotationY_Degrees(m_rotateSpeed.y);
	gameObject.lock()->transform->RollLocalRotationZ_Degrees(m_rotateSpeed.z);


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
		Dead(); //死
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
	float rotateSpeedRate = 5.0f;
	m_rotateSpeed = Vector3(ButiRandom::GetInt(-5, 5), ButiRandom::GetInt(-5, 5), ButiRandom::GetInt(-5, 5));
	m_rotateSpeed.Normalize();
	m_rotateSpeed *= rotateSpeedRate;
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

void ButiEngine::VolcanoRock::Dead()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->Dead();
	gameObject.lock()->SetIsRemove(true);
}
