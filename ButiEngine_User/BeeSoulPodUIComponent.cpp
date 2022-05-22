#include "stdafx_u.h"
#include "BeeSoulPodUIComponent.h"
#include "Player.h"

void ButiEngine::BeeSoulPodUIComponent::OnUpdate()
{
	Animation();
}

void ButiEngine::BeeSoulPodUIComponent::OnSet()
{
	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>(60);
	m_rate = 0.0f;
	m_previousRate = 0.0f;
	m_soulCount = 0;
	m_maxSoulCount = 10;
	m_previousMaxSoulCount = 10;
}

void ButiEngine::BeeSoulPodUIComponent::OnShowUI()
{
}

void ButiEngine::BeeSoulPodUIComponent::Start()
{
	m_vwp_meshDrawComponent = gameObject.lock()->GetGameComponent<MeshDrawComponent>(1);
	m_vwp_player = GetManager().lock()->GetGameObject("Player");
	m_vwp_playerComponent = m_vwp_player.lock()->GetGameComponent<Player>();
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
		m_previousMaxSoulCount = m_maxSoulCount;

		if (m_vwp_playerComponent.lock())
		{
			m_maxSoulCount = m_vwp_playerComponent.lock()->CalculateRequestExp();

			if (!m_vwp_playerComponent.lock()->IsMaxLevel())
			{
				auto levelUpUI = GetManager().lock()->AddObjectFromCereal("LevelUpUI");
				Vector3 screenPosition = GetCamera("main")->WorldToScreen(m_vwp_player.lock()->transform->GetWorldPosition());
				screenPosition.y += 100;
				screenPosition.z = 0;
				levelUpUI.lock()->transform->SetLocalPosition(screenPosition);
			}
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
		m_rate = 1.0f - ((float)m_previousMaxSoulCount - (float)m_keepSoulCount) * 0.1f;
	}

	auto lerpScale = 0.1f * GameDevice::WorldSpeed;
	m_previousRate = MathHelper::Lerp(m_previousRate, m_rate, lerpScale);

	m_vwp_meshDrawComponent.lock()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().ExInfo.y = 1.0 - m_previousRate;
}
