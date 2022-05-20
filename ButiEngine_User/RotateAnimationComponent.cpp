#include "stdafx_u.h"
#include "RotateAnimationComponent.h"

void ButiEngine::RotateAnimationComponent::OnUpdate()
{
	if (m_isRandomSpeed)
	{
		SetRandomSpeed();
	}
	m_theta += m_motionSpeed * GameDevice::WorldSpeed;

	Vector3 rotation = m_rotation;
	switch (m_rotateAxisType)
	{
	case 0:
		rotation.x = m_rotation.x + std::sin(m_theta) * m_amplitude;
		break;
	case 1:
		rotation.y = m_rotation.y + std::sin(m_theta) * m_amplitude;
		break;
	case 2:
		rotation.z = m_rotation.z + std::sin(m_theta) * m_amplitude;
		break;
	default:
		break;
	}
	
	gameObject.lock()->transform->SetLocalRotationX_Degrees(rotation.x);
	gameObject.lock()->transform->SetLocalRotationY_Degrees(rotation.y);
	gameObject.lock()->transform->SetLocalRotationZ_Degrees(rotation.z);
}

void ButiEngine::RotateAnimationComponent::OnSet()
{
	m_rotation = gameObject.lock()->transform->GetLocalRotation_Euler();

	m_theta = 0.0f;
	m_amplitude = 1.0f;
	m_motionSpeed = 0.01f;

	m_vlp_changeSpeedInterval = ObjectFactory::Create<RelativeTimer>(30);
	m_vlp_changeSpeedInterval->Start();
	m_targetSpeed = m_motionSpeed;
	m_minTargetSpeed = 0.03f;
	m_maxTargetSpeed = 0.3f;
	m_lerpSpeed = 0.1f;

	m_changeSpeedFrame = 30;
	m_rotateAxisType = 0;
}

void ButiEngine::RotateAnimationComponent::OnRemove()
{
}

void ButiEngine::RotateAnimationComponent::OnShowUI()
{
}

void ButiEngine::RotateAnimationComponent::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::RotateAnimationComponent::Clone()
{
	return ObjectFactory::Create<RotateAnimationComponent>();
}

void ButiEngine::RotateAnimationComponent::SetRandomSpeed()
{
	if (m_vlp_changeSpeedInterval->Update())
	{
		m_targetSpeed = ButiRandom::GetRandom(m_minTargetSpeed, m_maxTargetSpeed, 100);
	}
	m_motionSpeed = MathHelper::Lerp(m_motionSpeed, m_targetSpeed, m_lerpSpeed * GameDevice::WorldSpeed);
}
