#include "stdafx_u.h"
#include "KnockBack.h"

ButiEngine::KnockBack::KnockBack(const Vector3& arg_dir, const float arg_force, const std::int32_t arg_knockBackFrame)
{
	m_velocity = arg_dir * arg_force;
	m_startVelocity = m_velocity;
	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(arg_knockBackFrame);
	m_vlp_timer->Start();
}

void ButiEngine::KnockBack::OnUpdate()
{
	m_velocity = MathHelper::LerpPosition(m_startVelocity, Vector3Const::Zero, m_vlp_timer->GetPercent());
	gameObject.lock()->transform->Translate(m_velocity);


	if (m_vlp_timer->Update())
	{
		SetIsRemove(true);
	}
}

void ButiEngine::KnockBack::OnSet()
{
}

void ButiEngine::KnockBack::OnRemove()
{
}

void ButiEngine::KnockBack::OnShowUI()
{
}

void ButiEngine::KnockBack::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::KnockBack::Clone()
{
	return ObjectFactory::Create<KnockBack>();
}
