#include "stdafx_u.h"
#include "StageProgressUIComponent.h"

void ButiEngine::StageProgressUIComponent::OnUpdate()
{
	if (m_previousRate != m_rate)
	{
		m_previousRate = m_rate;
		gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().ExInfo.z = 1.0f - m_rate;
		gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color = Vector4(1, 1, 1, 1);
		m_vlp_timer->Reset();
		m_vlp_timer->Start();
	}

	if (m_vlp_timer->Update_continue())
	{
		gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color = m_defaultColor;
	}
	
}

void ButiEngine::StageProgressUIComponent::OnSet()
{
	m_vlp_timer = ObjectFactory::Create<AbsoluteTimer>(5);
	m_defaultColor = gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color;
	m_rate = 0;
	m_previousRate = 0;
}

void ButiEngine::StageProgressUIComponent::OnShowUI()
{
}

void ButiEngine::StageProgressUIComponent::Start()
{
	m_isChangeColor = false;
	gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().ExInfo.z = 1.0f;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::StageProgressUIComponent::Clone()
{
	return ObjectFactory::Create<StageProgressUIComponent>();
}
