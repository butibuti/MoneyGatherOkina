#include "stdafx_u.h"
#include "CameraShakeComponent.h"

#include "InputManager.h"

void ButiEngine::CameraShakeComponent::OnUpdate()
{
	if (InputManager::IsTriggerDecideKey())
	{
		ShakeStart(4, 40);
	}

	if (!m_startFlag)
	{
		return;
	}


	if (m_oneShakeCount <= 0)
	{
		m_oneShakeCount = m_oneShakeFrame;

		m_endRotate.x = ButiRandom::GetRandom(m_amplitude * 0.9f, m_amplitude, 100) * (float)GetTwoValue();
		m_endRotate.y = ButiRandom::GetRandom(m_amplitude * 0.9f, m_amplitude, 100) * (float)GetTwoValue();
		m_endRotate.z = ButiRandom::GetRandom(m_amplitude * 0.9f, m_amplitude, 100) * (float)GetTwoValue();

	}
	else
	{
		//•âŠÔ‚·‚é‚½‚ß‚ÌŒvŽZ
		float ratio = Easing::EaseInOutQuad(1.0f / (float)m_oneShakeCount);
		m_subRotate = (m_endRotate - m_startRotate) * ratio;

		m_oneShakeCount--;
	}

	m_startRotate = m_startRotate + m_subRotate;


	//m_moveRotate.x = ButiRandom::GetRandom(-m_amplitude, m_amplitude, 100);
	//m_moveRotate.y = ButiRandom::GetRandom(-m_amplitude, m_amplitude, 100);
	//m_moveRotate.z = ButiRandom::GetRandom(-m_amplitude, m_amplitude, 100);

	if (m_amplitude > 0)
	{
		m_amplitude -= m_subAmplitude;
	}
	else
	{
		ShakeStop();
		return;
	}

	Vector3 newRotate = m_defaultRotate + m_startRotate; // + m_moveRotate;

	gameObject.lock()->transform->SetLocalRotation(newRotate);
}

void ButiEngine::CameraShakeComponent::OnSet()
{
}

void ButiEngine::CameraShakeComponent::Start()
{
	m_startFlag = false;
	m_amplitude = 0.0f;
	m_subAmplitude = 0.0f;
	m_oneShakeFrame = 2;
	m_oneShakeCount = 0;
	m_moveRotate = Vector3(0, 0, 0);
	m_defaultRotate = Vector3(0, 0, 0);
	m_startRotate = Vector3(0, 0, 0);
	m_endRotate = Vector3(0, 0, 0);
	m_subRotate = Vector3(0, 0, 0);
}

void ButiEngine::CameraShakeComponent::ShakeStart(const float arg_amplitude)
{
	if (m_startFlag)
	{
		gameObject.lock()->transform->SetLocalRotation(m_defaultRotate);
		m_amplitude = 0.0f;
		m_subAmplitude = 0.0f;
	}
	m_amplitude = arg_amplitude;
	m_subAmplitude = m_amplitude * 0.2f;
	m_startFlag = true;
	m_defaultRotate = gameObject.lock()->transform->GetLocalRotation().GetEulerOneValue_local().ToDegrees();
}

void ButiEngine::CameraShakeComponent::ShakeStart(const float arg_amplitude, const std::int32_t arg_frame)
{
	if (m_startFlag)
	{
		gameObject.lock()->transform->SetLocalRotation(m_defaultRotate);
		m_amplitude = 0.0f;
		m_subAmplitude = 0.0f;
	}
	m_amplitude = arg_amplitude;
	m_subAmplitude = m_amplitude / (float)arg_frame;
	m_startFlag = true;
	m_defaultRotate = gameObject.lock()->transform->GetLocalRotation().GetEulerOneValue_local().ToDegrees();
}

void ButiEngine::CameraShakeComponent::ShakeStop()
{
	gameObject.lock()->transform->SetLocalRotation(m_defaultRotate);
	m_amplitude = 0.0f;
	m_subAmplitude = 0.0f;
	m_startFlag = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::CameraShakeComponent::Clone()
{
	return ObjectFactory::Create<CameraShakeComponent>();
}

std::int8_t ButiEngine::CameraShakeComponent::GetTwoValue()
{
	if (ButiRandom::GetRandom(0, 1))
	{
		return -1;
	}
	else
	{
		return 1;
	}
}
