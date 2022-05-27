#include "stdafx_u.h"
#include "ShakeComponent.h"
#include "InputManager.h"

float shakePower = 0.1f;

void ButiEngine::ShakeComponent::OnUpdate()
{
	ShakeLimited();

	if (!m_isShake || m_amplitude <= 0.0f)
	{
		return;
	}

	if (!m_isMove)
	{
		if (m_isRotateShake)
		{
			Vector3 randomRotate;
			randomRotate.x = ButiRandom::GetRandom(-5, 5);
			randomRotate.y = ButiRandom::GetRandom(-5, 5);
			randomRotate.z = ButiRandom::GetRandom(-5, 5);

			if (m_isAbsolute)
			{
				m_moveRotate = randomRotate * m_amplitude * 2.0f * m_rotateAmplitude;
			}
			else
			{
				m_moveRotate = randomRotate * m_amplitude * 2.0f * m_rotateAmplitude * GameDevice::WorldSpeed;
			}
			
			m_moveRotate *= m_shakeAxis;
		}

		if (m_isPositionShake)
		{
			Vector3 randomPosition;
			randomPosition.x = ButiRandom::GetRandom(-5, 5);
			randomPosition.y = ButiRandom::GetRandom(-5, 5);
			randomPosition.z = ButiRandom::GetRandom(-5, 5);
			randomPosition.Normalize();

			if (m_isAbsolute)
			{
				m_movePos = randomPosition * m_amplitude * 0.1f * m_positionAmplitude;
			}
			else
			{
				m_movePos = randomPosition * m_amplitude * 0.1f * m_positionAmplitude * GameDevice::WorldSpeed;
			}
		}

		if (m_isScaleShake)
		{
			float randomScale = ButiRandom::GetRandom(-10, 10) * 0.1f;
			if (m_isAbsolute)
			{
				m_moveScale = randomScale * m_amplitude * m_scaleAmplitude;
			}
			else
			{
				m_moveScale = randomScale * m_amplitude * m_scaleAmplitude * GameDevice::WorldSpeed;
			}

		}

	}
	else
	{
		m_moveRotate = -m_moveRotate;
		m_movePos = -m_movePos;
		m_moveScale = -m_moveScale;
	}
	
	if (m_isRotateShake)
	{
		m_currentRotate = gameObject.lock()->transform->GetLocalRotation().GetEulerOneValue_local().ToDegrees();
		m_currentRotate = m_currentRotate + m_moveRotate;
		gameObject.lock()->transform->SetLocalRotation(m_currentRotate);
	}

	if (m_isPositionShake)
	{
		m_currentPos = gameObject.lock()->transform->GetLocalPosition();
		m_currentPos = m_currentPos + m_movePos;
		gameObject.lock()->transform->SetLocalPosition(m_currentPos);
	}

	if (m_isScaleShake)
	{
		m_currentScale = gameObject.lock()->transform->GetLocalScale();
		m_currentScale = m_currentScale + m_moveScale;
		gameObject.lock()->transform->SetLocalScale(m_currentScale);
	}

	m_isMove = !m_isMove;

}

void ButiEngine::ShakeComponent::OnSet()
{
	m_vlp_shakeTimer = ObjectFactory::Create<AbsoluteTimer>(2);
	m_isShake = false;
	m_isMove = false;
	m_isShakeLimited = false;
	m_isRotateShake = true;
	m_isPositionShake = true;
	m_isScaleShake = false;
	m_isAbsolute = false;
	m_currentRotate = gameObject.lock()->transform->GetLocalRotation().GetEulerOneValue_local().ToDegrees();
	m_moveRotate = Vector3(0, 0, 0);
	m_currentPos = gameObject.lock()->transform->GetLocalPosition();
	m_movePos = Vector3(0, 0, 0);
	m_currentScale = gameObject.lock()->transform->GetLocalScale();
	m_moveScale = Vector3(0, 0, 0);
	m_shakeAxis = Vector3(1, 1, 1);
	m_amplitude = 0.0f;
	m_rotateAmplitude = 1.0f;
	m_positionAmplitude = 1.0f;
	m_scaleAmplitude = 1.0f;
}

void ButiEngine::ShakeComponent::Start()
{

}

void ButiEngine::ShakeComponent::OnShowUI()
{
	GUI::BulletText("ShakePower");
	GUI::DragFloat("##ShakePower", &shakePower, 0.01f, 0.0f, 1.0f);
}

void ButiEngine::ShakeComponent::Shake(const float arg_amplitude, const std::int32_t arg_shakeFrame)
{
	m_vlp_shakeTimer->Reset();
	m_vlp_shakeTimer->ChangeCountFrame(arg_shakeFrame);
	m_vlp_shakeTimer->Start();

	ShakeStop();
	m_isMove = false;
	m_isShake = true;
	m_isShakeLimited = true;
	m_currentRotate = gameObject.lock()->transform->GetLocalRotation().GetEulerOneValue_local().ToDegrees();
	m_moveRotate = Vector3(0, 0, 0);
	m_currentPos = gameObject.lock()->transform->GetLocalPosition();
	m_movePos = Vector3(0, 0, 0);
	m_currentScale = gameObject.lock()->transform->GetLocalScale();
	m_moveScale = Vector3(0, 0, 0);
	m_amplitude = arg_amplitude;
}

void ButiEngine::ShakeComponent::ShakeStart()
{
	ShakeStop();
	m_isMove = false;
	m_isShake = true;
	m_currentRotate = gameObject.lock()->transform->GetLocalRotation().GetEulerOneValue_local().ToDegrees();
	m_moveRotate = Vector3(0, 0, 0);
	m_currentPos = gameObject.lock()->transform->GetLocalPosition();
	m_movePos = Vector3(0, 0, 0);
	m_currentScale = gameObject.lock()->transform->GetLocalScale();
	m_moveScale = Vector3(0, 0, 0);
}

void ButiEngine::ShakeComponent::ShakeStop()
{
	m_isShake = false;
	m_isShakeLimited = false;
	if (m_isMove)
	{
		if (m_isRotateShake)
		{
			m_moveRotate = -m_moveRotate;
			m_currentRotate = gameObject.lock()->transform->GetLocalRotation().GetEulerOneValue_local().ToDegrees();
			m_currentRotate = m_currentRotate + m_moveRotate;
			gameObject.lock()->transform->SetLocalRotation(m_currentRotate);
		}
		
		if (m_isPositionShake)
		{
			m_movePos = -m_movePos;
			m_currentPos = gameObject.lock()->transform->GetLocalPosition();
			m_currentPos = m_currentPos + m_movePos;
			gameObject.lock()->transform->SetLocalPosition(m_currentPos);
		}

		if (m_isScaleShake)
		{
			m_moveScale = -m_moveScale;
			m_currentScale = gameObject.lock()->transform->GetLocalScale();
			m_currentScale = m_currentScale + m_moveScale;
			gameObject.lock()->transform->SetLocalScale(m_currentScale);
		}
		
		m_isMove = false;
	}
}

void ButiEngine::ShakeComponent::SetShakePower(const float arg_amplitude)
{
	m_amplitude = arg_amplitude;
}

void ButiEngine::ShakeComponent::ShakeLimited()
{
	if (!m_isShakeLimited) { return; }
	if (m_vlp_shakeTimer->Update())
	{
		ShakeStop();
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::ShakeComponent::Clone()
{
	return ObjectFactory::Create<ShakeComponent>();
}
