#include "stdafx_u.h"
#include "BeeSoulPodUIComponent.h"
#include "Player.h"

void ButiEngine::BeeSoulPodUIComponent::OnUpdate()
{
	Animation();
}

void ButiEngine::BeeSoulPodUIComponent::OnSet()
{
	m_rate = 0.0f;
	m_previousRate = 0.0f;
	m_soulCount = 0;
	m_maxSoulCount = 10;
}

void ButiEngine::BeeSoulPodUIComponent::OnShowUI()
{
}

void ButiEngine::BeeSoulPodUIComponent::Start()
{
	m_vwp_meshDrawComponent = gameObject.lock()->GetGameComponent<MeshDrawComponent>(1);
	m_vwp_player = GetManager().lock()->GetGameObject("Player");
	m_vwp_playerComponent = m_vwp_player.lock()->GetGameComponent<Player>();
	m_maxSoulCount = m_vwp_playerComponent.lock()->GetMaxSoulCount();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::BeeSoulPodUIComponent::Clone()
{
	return ObjectFactory::Create<BeeSoulPodUIComponent>();
}

void ButiEngine::BeeSoulPodUIComponent::AddSoulCount()
{
	m_soulCount++;
	if (m_soulCount > m_maxSoulCount)
	{
		m_soulCount = m_maxSoulCount;
	}
}

void ButiEngine::BeeSoulPodUIComponent::Animation()
{
	m_rate = (float)m_soulCount / (float)m_maxSoulCount;

	auto lerpScale = 0.1f * GameDevice::WorldSpeed;
	m_previousRate = MathHelper::Lerp(m_previousRate, m_rate, lerpScale);

	m_vwp_meshDrawComponent.lock()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().ExInfo.y = 1.0 - m_previousRate;
}
