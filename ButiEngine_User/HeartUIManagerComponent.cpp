#include "stdafx_u.h"
#include "HeartUIManagerComponent.h"
#include "Player.h"

void ButiEngine::HeartUIManagerComponent::OnUpdate()
{
	auto playerLife = m_vwp_playerComponent.lock()->GetLife();
	if (m_lifeCount > playerLife)
	{
		m_vec_vlp_heartUI[m_lifeCount - 1]->SetIsRemove(true);
		m_vec_vlp_heartUI[m_lifeCount - 1] = Value_ptr<GameObject>();
		m_lifeCount = playerLife;
	}
	else if (m_lifeCount < playerLife)
	{
		for (std::int8_t i = 0; i < playerLife; i++)
		{
			if (m_vec_vlp_heartUI[i] == nullptr)
			{
				m_vec_vlp_heartUI[i] = GetManager().lock()->AddObjectFromCereal("HeartUI");
				auto position = m_heartUIPosition;
				position.x += (m_heartUIScale.x + m_space) * i;
				m_vec_vlp_heartUI[i]->transform->SetLocalPosition(position);
			}
		}
		m_lifeCount = playerLife;
	}
}

void ButiEngine::HeartUIManagerComponent::OnSet()
{
}

void ButiEngine::HeartUIManagerComponent::OnShowUI()
{
}

void ButiEngine::HeartUIManagerComponent::Start()
{
	m_heartUIPosition = Vector3(-870, 450, 0);
	m_heartUIScale = Vector3(100, 100, 1);
	m_space = 15.0f;

	m_vwp_playerComponent = GetManager().lock()->GetGameObject("Player").lock()->GetGameComponent<Player>();	
	m_lifeCount = m_vwp_playerComponent.lock()->GetLife();
	for (std::int8_t i = 0; i < m_lifeCount; i++)
	{
		m_vec_vlp_heartUI.push_back(GetManager().lock()->AddObjectFromCereal("HeartUI"));
		auto position = m_heartUIPosition;
		position.x += (m_heartUIScale.x + m_space) * i;
		m_vec_vlp_heartUI[i]->transform->SetLocalPosition(position);
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::HeartUIManagerComponent::Clone()
{
	return ObjectFactory::Create<HeartUIManagerComponent>();
}

void ButiEngine::HeartUIManagerComponent::ResetHeartUI()
{
	
}
