#include "stdafx_u.h"
#include "FloatMotionComponent.h"

void ButiEngine::FloatMotionComponent::OnUpdate()
{
	if (m_isRandomSpeed)
	{
		SetRandomSpeed();
	}
	m_theta += m_motionSpeed * GameDevice::WorldSpeed;
	gameObject.lock()->transform->SetWorldPostionY(std::sin(m_theta) * m_amplitude);
}

void ButiEngine::FloatMotionComponent::OnSet()
{
}

void ButiEngine::FloatMotionComponent::OnShowUI()
{
	GUI::BulletText("Amplitude");
	GUI::DragFloat("##amplitude", &m_amplitude, 0.01f, 0.0f, 10.0f);
	GUI::BulletText("Speed");
	GUI::DragFloat("##speed", &m_motionSpeed, 0.01f, 0.0f, 1.0f);
	GUI::Checkbox("isRandom", &m_isRandomSpeed);
	GUI::BulletText("ChangeSpeedInterval");
	if (GUI::DragInt("interval", &m_changeSpeedFrame, 1.0f, 0, 120))
	{
		m_vlp_changeSpeedInterval->ChangeCountFrame(m_changeSpeedFrame);
	}
	GUI::BulletText("MinTargetSpeed");
	GUI::DragFloat("##minspeed", &m_minTargetSpeed, 0.01f, 0.0f, 10.0f);
	GUI::BulletText("MaxTargetSpeed");
	GUI::DragFloat("##maxspeed", &m_maxTargetSpeed, 0.01f, 0.0f, 10.0f);
	GUI::BulletText("LerpSpeed");
	GUI::DragFloat("##lerp", &m_lerpSpeed, 0.01f, 0.0f, 1.0f);
}

void ButiEngine::FloatMotionComponent::Start()
{
	m_theta = 0.0f;
	m_amplitude = 0.1f;
	m_motionSpeed = 0.01f;

	m_vlp_changeSpeedInterval = ObjectFactory::Create<RelativeTimer>(30);
	m_vlp_changeSpeedInterval->Start();
	m_isRandomSpeed = true;
	m_targetSpeed = m_motionSpeed;
	m_minTargetSpeed = 0.03f;
	m_maxTargetSpeed = 0.3f;
	m_lerpSpeed = 0.1f;

	m_changeSpeedFrame = 30;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FloatMotionComponent::Clone()
{
	return ObjectFactory::Create<FloatMotionComponent>();
}

void ButiEngine::FloatMotionComponent::SetRandomSpeed()
{
	if (m_vlp_changeSpeedInterval->Update())
	{
		m_targetSpeed = ButiRandom::GetRandom(m_minTargetSpeed, m_maxTargetSpeed, 100);
	}
	m_motionSpeed = MathHelper::Lerp(m_motionSpeed, m_targetSpeed, m_lerpSpeed * GameDevice::WorldSpeed);
}
