#include "stdafx_u.h"
#include "FadeOutComponent.h"

void ButiEngine::FadeOutComponent::OnUpdate()
{
	if (m_alpha < 1.0f)
	{
		float moveAlpha = 1.0f / 100.0f;
		m_alpha += moveAlpha;
	}
	gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color.w = m_alpha;

}

void ButiEngine::FadeOutComponent::OnSet()
{
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
