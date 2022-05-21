#include "stdafx_u.h"
#include "FadeOutComponent.h"

void ButiEngine::FadeOutComponent::OnUpdate()
{
	if (!m_isFade)
	{
		if (m_alpha < 1.0f)
		{		
			m_alpha += m_moveAlpha;
		}
	}
	else
	{
		if (m_alpha > 0.0f)
		{
			m_alpha -= m_moveAlpha;
		}
	}

	gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color.w = m_alpha;

}

void ButiEngine::FadeOutComponent::OnSet()
{
	m_moveAlpha = 1.0f / 60.0f;
	m_isFade = false;
}

void ButiEngine::FadeOutComponent::OnShowUI()
{
}

void ButiEngine::FadeOutComponent::Start()
{
	m_alpha = 0.0f;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FadeOutComponent::Clone()
{
	return ObjectFactory::Create<FadeOutComponent>();
}
