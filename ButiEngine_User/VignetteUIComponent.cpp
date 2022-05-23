#include "stdafx_u.h"
#include "VignetteUIComponent.h"

void ButiEngine::VignetteUIComponent::OnUpdate()
{
	if (!m_vlp_timer->Update_continue())
	{
		if (m_isChange)
		{
			m_alpha += m_moveAlpha * 2.0f * GameDevice::WorldSpeed;
		}
		else
		{
			m_alpha -= m_moveAlpha * GameDevice::WorldSpeed;
		}
	}
	else
	{
		if (m_isChange)
		{
			m_isChange = false;
			m_vlp_timer->Reset();
		}
	}

	if (m_alpha > 1.0f)
	{
		m_alpha = 1.0f;
	}
	else if (m_alpha < 0.0f)
	{
		m_alpha = 0.0f;
	}


	gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color.w = m_alpha;
}

void ButiEngine::VignetteUIComponent::OnSet()
{
	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(25);
	m_alpha = 0;
	m_moveAlpha = 1.0f / 25.0f;
	m_isChange = false;
}

void ButiEngine::VignetteUIComponent::Start()
{
	m_vlp_timer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::VignetteUIComponent::Clone()
{
	return ObjectFactory::Create<VignetteUIComponent>();
}

void ButiEngine::VignetteUIComponent::StartAlphaAnimation()
{
	m_vlp_timer->Reset();
	m_isChange = true;
}
