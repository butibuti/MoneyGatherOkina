#include "stdafx_u.h"
#include "ShakeComponent.h"
#include "InputManager.h"

float shakePower = 0.1f;

void ButiEngine::ShakeComponent::OnUpdate()
{
	//if (InputManager::IsTriggerLeftKey())
	//{
	//	ShakeStart();
	//}
	//else if (InputManager::IsTriggerRightKey())
	//{
	//	ShakeStop();
	//}

	//SetShakePower(shakePower);

	if (!m_isShake || m_amplitude <= 0.0f)
	{
		return;
	}

	if (!m_isMove)
	{
		Vector3 randomRotate;
		randomRotate.x = ButiRandom::GetRandom(-5, 5);
		randomRotate.y = ButiRandom::GetRandom(-5, 5);
		randomRotate.z = ButiRandom::GetRandom(-5, 5);
		
		m_moveRotate = randomRotate * m_amplitude * 2.0f;

		Vector3 randomPosition;
		randomPosition.x = ButiRandom::GetRandom(-5, 5);
		randomPosition.y = ButiRandom::GetRandom(-5, 5);
		randomPosition.z = ButiRandom::GetRandom(-5, 5);
		randomPosition.Normalize();

		m_movePos = randomPosition * m_amplitude * 0.1f;
	}
	else
	{
		m_moveRotate = -m_moveRotate;
		m_movePos = -m_movePos;
	}
	
	m_currentRotate = gameObject.lock()->transform->GetLocalRotation().GetEulerOneValue_local().ToDegrees();
	m_currentRotate = m_currentRotate + m_moveRotate;
	gameObject.lock()->transform->SetLocalRotation(m_currentRotate);

	m_currentPos = gameObject.lock()->transform->GetLocalPosition();
	m_currentPos = m_currentPos + m_movePos;
	gameObject.lock()->transform->SetLocalPosition(m_currentPos);

	m_isMove = !m_isMove;

}

void ButiEngine::ShakeComponent::OnSet()
{
	m_isShake = false;
	m_isMove = false;
	m_currentRotate = gameObject.lock()->transform->GetLocalRotation().GetEulerOneValue_local().ToDegrees();
	m_moveRotate = Vector3(0, 0, 0);
	m_currentPos = gameObject.lock()->transform->GetLocalPosition();
	m_movePos = Vector3(0, 0, 0);
	m_amplitude = 0.5f;
}

void ButiEngine::ShakeComponent::Start()
{

}

void ButiEngine::ShakeComponent::OnShowUI()
{
	GUI::BulletText("ShakePower");
	GUI::DragFloat("##ShakePower", &shakePower, 0.01f, 0.0f, 1.0f);
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

}

void ButiEngine::ShakeComponent::ShakeStop()
{
	m_isShake = false;
	if (m_isMove)
	{
		m_moveRotate = -m_moveRotate;
		m_currentRotate = gameObject.lock()->transform->GetLocalRotation().GetEulerOneValue_local().ToDegrees();
		m_currentRotate = m_currentRotate + m_moveRotate;
		gameObject.lock()->transform->SetLocalRotation(m_currentRotate);
		
		m_movePos = -m_movePos;
		m_currentPos = gameObject.lock()->transform->GetLocalPosition();
		m_currentPos = m_currentPos + m_movePos;
		gameObject.lock()->transform->SetLocalPosition(m_currentPos);
		
		m_isMove = false;
	}
}

void ButiEngine::ShakeComponent::SetShakePower(const float arg_amplitude)
{
	m_amplitude = arg_amplitude;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::ShakeComponent::Clone()
{
	return ObjectFactory::Create<ShakeComponent>();
}
