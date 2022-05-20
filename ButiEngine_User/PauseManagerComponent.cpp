#include "stdafx_u.h"
#include "PauseManagerComponent.h"
#include "WorldSpeedManager.h"
#include "InputManager.h"

void ButiEngine::PauseManagerComponent::OnUpdate()
{
	if (InputManager::IsTriggerPauseKey())
	{
		m_isPause = !m_isPause;
		if (m_isPause)
		{
			m_vwp_worldSpeedManagerComponent.lock()->SetSpeed(0.0f);
		}
		else
		{
			m_vwp_worldSpeedManagerComponent.lock()->SetSpeed(1.0f);
		}
	}
}

void ButiEngine::PauseManagerComponent::OnSet()
{
}

void ButiEngine::PauseManagerComponent::OnShowUI()
{
}

void ButiEngine::PauseManagerComponent::Start()
{
	m_vwp_worldSpeedManagerComponent = GetManager().lock()->GetGameObject("WorldSpeedManager").lock()->GetGameComponent<WorldSpeedManager>();
	m_isPause = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::PauseManagerComponent::Clone()
{
	return ObjectFactory::Create<PauseManagerComponent>();
}

void ButiEngine::PauseManagerComponent::Pause()
{
}
