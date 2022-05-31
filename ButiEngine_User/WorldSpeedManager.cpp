#include "stdafx_u.h"
#include "WorldSpeedManager.h"
#include "ProgressPointUI.h"

ButiEngine::Value_ptr<ButiEngine::Timer> ButiEngine::WorldSpeedManager::m_vlp_timer;
bool ButiEngine::WorldSpeedManager::m_isChangeSpeed;

void ButiEngine::WorldSpeedManager::OnUpdate()
{
#ifdef DEBUG
	if (GameDevice::GetInput()->TriggerKey(Keys::K))
	{
		SetSpeed(0.0f, 180);
	}
#endif // DEBUG
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
	GUI::BulletText("WorldSpeed");
	GUI::DragFloat("##speed", &GameDevice::WorldSpeed, 0.01f, 0.0f, 1.0f);
}

void ButiEngine::WorldSpeedManager::Start()
{
	ProgressPointUI::ResetZShift();

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
