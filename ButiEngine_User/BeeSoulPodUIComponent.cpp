#include "stdafx_u.h"
#include "BeeSoulPodUIComponent.h"
#include "Player.h"

void ButiEngine::BeeSoulPodUIComponent::OnUpdate()
{
	Animation();
}

void ButiEngine::BeeSoulPodUIComponent::OnSet()
{
	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>(90);
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
	m_vwp_playerComponent = GetManager().lock()->GetGameObject("Player").lock()->GetGameComponent<Player>();
	m_vlp_waitTimer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::BeeSoulPodUIComponent::Clone()
{
	return ObjectFactory::Create<BeeSoulPodUIComponent>();
}

void ButiEngine::BeeSoulPodUIComponent::AddSoulCount()
{
	m_soulCount++;
	if (m_soulCount >= m_maxSoulCount)
	{
		m_vlp_waitTimer->Reset();
		m_keepSoulCount = m_maxSoulCount;
		if (m_vwp_playerComponent.lock())
		{
			m_maxSoulCount = m_vwp_playerComponent.lock()->CalculateRequestExp();
		}
	}
}

void ButiEngine::BeeSoulPodUIComponent::Animation()
{
	if (m_vlp_waitTimer->Update_continue())
	{
		m_rate = 1.0f - ((float)m_maxSoulCount - (float)m_soulCount) * 0.1f;
	}
	else
	{
		m_rate = 1.0f - ((float)m_maxSoulCount - (float)m_keepSoulCount * 2.0f) * 0.1f;
	}

	auto lerpScale = 0.1f * GameDevice::WorldSpeed;
	m_previousRate = MathHelper::Lerp(m_previousRate, m_rate, lerpScale);

	m_vwp_meshDrawComponent.lock()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().ExInfo.y = 1.0 - m_previousRate;
}
