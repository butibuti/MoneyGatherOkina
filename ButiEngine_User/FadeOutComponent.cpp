#include "stdafx_u.h"
#include "FadeOutComponent.h"

void ButiEngine::FadeOutComponent::OnUpdate()
{
	if (!m_isFadeIn)
	{
		if (m_alpha < m_maxAlpha)
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
	m_maxAlpha = 1.0f;
	m_moveAlpha = 1.0f / 60.0f;
	m_isFadeIn = false;
}

void ButiEngine::FadeOutComponent::OnShowUI()
{
}

void ButiEngine::FadeOutComponent::Start()
{
	m_alpha = gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color.w;
	gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color = Vector4(0.074f, 0, 0.192f, m_alpha);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FadeOutComponent::Clone()
{
	return ObjectFactory::Create<FadeOutComponent>();
}

void ButiEngine::FadeOutComponent::SetPositionZ(const float arg_positionZ)
{
	gameObject.lock()->transform->SetLocalPositionZ(arg_positionZ);
}

void ButiEngine::FadeOutComponent::SetColor(const Vector3& arg_color)
{
	auto objInfo = gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation");
	objInfo->Get().color.x = arg_color.x;
	objInfo->Get().color.y = arg_color.y;
	objInfo->Get().color.z = arg_color.z;
}

bool ButiEngine::FadeOutComponent::IsFadeAnimation() const
{
	return m_isFadeIn ? gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color.w > 0.0f
		:gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color.w < 1.0f;
}
