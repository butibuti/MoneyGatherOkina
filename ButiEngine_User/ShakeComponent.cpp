#include "stdafx_u.h"
#include "ShakeComponent.h"

void ButiEngine::ShakeComponent::OnUpdate()
{
	if (!m_shakeFlag)
	{
		return;
	}



}

void ButiEngine::ShakeComponent::OnSet()
{
}

void ButiEngine::ShakeComponent::Start()
{
	m_shakeFlag = false;
}

void ButiEngine::ShakeComponent::ShakeStart(const float arg_amplitude)
{
	m_shakeFlag = true;
	m_defaultRotate = gameObject.lock()->transform->GetLocalRotation().GetEulerOneValue_local().ToDegrees();
	m_amplitude = arg_amplitude;

}

void ButiEngine::ShakeComponent::ShakeStop()
{
	m_shakeFlag = false;
	gameObject.lock()->transform->SetLocalRotation(m_defaultRotate);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::ShakeComponent::Clone()
{
	return ObjectFactory::Create<ShakeComponent>();
}
