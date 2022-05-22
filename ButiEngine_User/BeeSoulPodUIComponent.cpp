#include "stdafx_u.h"
#include "BeeSoulPodUIComponent.h"

void ButiEngine::BeeSoulPodUIComponent::OnUpdate()
{
	Animation();
}

void ButiEngine::BeeSoulPodUIComponent::OnSet()
{
	m_rate = 0.0f;
	m_previousRate = 0.0f;
}

void ButiEngine::BeeSoulPodUIComponent::OnShowUI()
{
}

void ButiEngine::BeeSoulPodUIComponent::Start()
{
	m_vwp_meshDrawComponent = gameObject.lock()->GetGameComponent<MeshDrawComponent>(1);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::BeeSoulPodUIComponent::Clone()
{
	return ObjectFactory::Create<BeeSoulPodUIComponent>();
}

void ButiEngine::BeeSoulPodUIComponent::SetExpRate(const float arg_expRate)
{
	m_rate = arg_expRate;
}

void ButiEngine::BeeSoulPodUIComponent::Animation()
{
	auto lerpScale = 0.05f * GameDevice::WorldSpeed;
	m_previousRate = MathHelper::Lerp(m_previousRate, m_rate, lerpScale);

	m_vwp_meshDrawComponent.lock()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().ExInfo.y = 1.0 - m_previousRate;
}
