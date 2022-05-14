#include "stdafx_u.h"
#include "WorldSpeedManager.h"

ButiEngine::Value_ptr<ButiEngine::Timer> ButiEngine::WorldSpeedManager::m_vlp_timer;
bool ButiEngine::WorldSpeedManager::m_isChangeSpeed;

void ButiEngine::WorldSpeedManager::OnUpdate()
{
	if (GameDevice::GetInput()->TriggerKey(Keys::K))
	{
		SetSpeed(0.0f, 30);
	}
	if (m_isChangeSpeed)
	{
		OnChangeSpeed();
	}
}

void ButiEngine::WorldSpeedManager::OnSet()
{
}

void ButiEngine::WorldSpeedManager::OnShowUI()
{
}

void ButiEngine::WorldSpeedManager::Start()
{
	m_vlp_timer = ObjectFactory::Create<AbsoluteTimer>(0);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::WorldSpeedManager::Clone()
{
	return ObjectFactory::Create<WorldSpeedManager>();
}

void ButiEngine::WorldSpeedManager::OnChangeSpeed()
{
	if (m_vlp_timer->Update())
	{
		GameDevice::WorldSpeed = 1.0f;

		m_vlp_timer->Stop();
		m_vlp_timer->Reset();

		m_isChangeSpeed = false;
	}
}
