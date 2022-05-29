#include "stdafx_u.h"
#include "WingAnimation.h"
#include "Player.h"
#include "Worker.h"

void ButiEngine::WingAnimation::OnUpdate()
{
	float vibrationRate = 0.0f;
	if (m_isPlayer)
	{
		vibrationRate = m_vwp_player.lock()->GetVibrationRate();
	}
	else if (m_isWorker)
	{
		vibrationRate = m_vwp_worker.lock()->GetVibrationRate();
	}
	vibrationRate = min(vibrationRate, 1.0f);

	m_flapSpeed = MathHelper::Lerp(m_minFlapSpeed, m_maxFlapSpeed, vibrationRate) * GameDevice::WorldSpeed;

	m_theta += m_flapSpeed;

	m_vwp_leftWing.lock()->SetLocalRotationZ_Degrees(90.0f * std::sin(m_theta));
	m_vwp_rightWing.lock()->SetLocalRotationZ_Degrees(-90.0f * std::sin(m_theta));
}

void ButiEngine::WingAnimation::OnSet()
{
}

void ButiEngine::WingAnimation::OnRemove()
{
}

void ButiEngine::WingAnimation::OnShowUI()
{
}

void ButiEngine::WingAnimation::Start()
{
	m_vwp_player = m_vwp_parent.lock()->GetGameComponent<Player>();
	m_vwp_worker = m_vwp_parent.lock()->GetGameComponent<Worker>();

	m_isPlayer = m_vwp_player.lock();
	m_isWorker = m_vwp_worker.lock();

	m_flapSpeed = 1.0f;
	m_minFlapSpeed = 1.0f;
	m_maxFlapSpeed = 2.0f;
	m_theta = 0.0f;

	m_vwp_leftWing = gameObject.lock()->GetGameComponent<MeshDrawComponent>(1)->GetTransform();
	m_vwp_rightWing = gameObject.lock()->GetGameComponent<MeshDrawComponent>(2)->GetTransform();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::WingAnimation::Clone()
{
	return ObjectFactory::Create<WingAnimation>();
}
