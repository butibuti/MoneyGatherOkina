#include "stdafx_u.h"
#include "VolcanoRock.h"
#include "ParticleGenerater.h"

void ButiEngine::VolcanoRock::OnUpdate()
{
	m_velocity.y -= m_gravity;
	Vector3 velocity = m_velocity;
	velocity.Normalize();
	velocity.x *= 0.1f;
	velocity.y *= 0.25f;
	velocity.z *= 0.1f;

	gameObject.lock()->transform->Translate(velocity);



	//if (m_life > 0)
	//{
	//	m_life--;
	//}
	//else
	//{
	//	//ライフが消えたら爆発エフェクトを出して死ぬ
	//	//ここで爆発エフェクト発生
	//	auto position = gameObject.lock()->transform->GetWorldPosition();
	//	m_vwp_particleGenerater.lock()->ExplosionParticles(position);
	//	gameObject.lock()->SetIsRemove(true); //死
	//}
}

void ButiEngine::VolcanoRock::OnSet()
{
}

void ButiEngine::VolcanoRock::OnShowUI()
{
}

void ButiEngine::VolcanoRock::Start()
{
	m_gravity = 0.05f;
	m_life = 180;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::VolcanoRock::Clone()
{
	return ObjectFactory::Create<VolcanoRock>();
}

void ButiEngine::VolcanoRock::SetVelocity(const Vector3& arg_velocity)
{
	m_velocity = arg_velocity;
}
