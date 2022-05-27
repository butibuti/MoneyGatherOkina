#include "stdafx_u.h"
#include "KnockBack.h"

ButiEngine::KnockBack::KnockBack(const Vector3& arg_dir, const float arg_force, const bool arg_isGravity, const std::int32_t arg_knockBackFrame)
{
	m_velocity = arg_dir * arg_force;
	m_startVelocity = m_velocity;
	m_isGravity = arg_isGravity;
	m_velocityY = arg_dir.y * arg_force;
	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(arg_knockBackFrame);
	m_vlp_timer->Start();
}

void ButiEngine::KnockBack::OnUpdate()
{
	float progress = m_vlp_timer->GetPercent() * GameDevice::WorldSpeed;
	progress = min(progress, 1.0f);
	m_velocity = MathHelper::LerpPosition(m_startVelocity, Vector3Const::Zero, progress);

	if (m_isGravity)
	{
		constexpr float gravity = 0.015f;
		m_velocityY -= gravity * GameDevice::WorldSpeed;
		m_velocity.y = m_velocityY;

		gameObject.lock()->transform->Translate(m_velocity * GameDevice::WorldSpeed);
		Vector3 pos = gameObject.lock()->transform->GetWorldPosition();

		if (pos.y < 0.0f || m_vlp_timer->Update())
		{
			gameObject.lock()->transform->SetWorldPostionY(0.0f);
			SetIsRemove(true);
		}
	}
	else
	{
		gameObject.lock()->transform->Translate(m_velocity * GameDevice::WorldSpeed);

		if (m_vlp_timer->Update())
		{
			SetIsRemove(true);
		}
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
