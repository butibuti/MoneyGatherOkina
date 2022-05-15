#include "stdafx_u.h"
#include "GameOverManagerComponent.h"
#include "InputManager.h"

void ButiEngine::GameOverManagerComponent::OnUpdate()
{
	InputSelect();
	if (m_vlp_waitTimer->Update())
	{
		m_vlp_waitTimer->Stop();
		m_isNext = true;
		AppearUI();
	}
}

void ButiEngine::GameOverManagerComponent::OnSet()
{
	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>();
}

void ButiEngine::GameOverManagerComponent::OnShowUI()
{
}

void ButiEngine::GameOverManagerComponent::Start()
{
	m_vlp_waitTimer->Start();
	m_vlp_waitTimer->ChangeCountFrame(120);
	GetManager().lock()->AddObjectFromCereal("FadeOutUI");
	m_isRetry = true;
	m_isNext = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GameOverManagerComponent::Clone()
{
	return ObjectFactory::Create<GameOverManagerComponent>();
}

void ButiEngine::GameOverManagerComponent::AppearUI()
{
	GetManager().lock()->AddObjectFromCereal("GameOverUI");
	GetManager().lock()->AddObjectFromCereal("RetryUI");
	GetManager().lock()->AddObjectFromCereal("NextTitleUI");
}

void ButiEngine::GameOverManagerComponent::InputSelect()
{
	if (!m_isNext) { return; }

	if (InputManager::IsTriggerLeftKey() || InputManager::IsTriggerRightKey())
	{
		m_isRetry = !m_isRetry;
	}

	if (InputManager::IsTriggerDecideKey())
	{
		m_isNext = false;
	}
}
