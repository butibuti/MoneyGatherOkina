#include "stdafx_u.h"
#include "PauseManagerComponent.h"
#include "WorldSpeedManager.h"
#include "ExpansionAnimationComponent.h"
#include "InputManager.h"
#include "FadeOutComponent.h"

void ButiEngine::PauseManagerComponent::OnUpdate()
{
	InputSelect();

	if (m_vlp_waitTimer->Update_continue() && InputManager::IsTriggerPauseKey())
	{
		Reset();
		m_vlp_waitTimer->Reset();
		m_vlp_appearTimer->Reset();
		m_isPause = !m_isPause;
		if (m_isPause)
		{
			m_vwp_fadeOutComponent.lock()->SetIsFade(false);
			m_vwp_worldSpeedManagerComponent.lock()->SetSpeed(0.0f);
		}
		else
		{
			m_vwp_fadeOutComponent.lock()->SetIsFade(true);
			if (m_vwp_expansionPauseUIComponent.lock())
			{
				m_vwp_expansionPauseUIComponent.lock()->SetIsActive(true);
				m_vwp_expansionPauseWindowUIComponent.lock()->SetIsActive(true);
				m_vwp_expansionPauseUIComponent.lock()->SetScale(Vector3(0, 0, 1));
				m_vwp_expansionPauseWindowUIComponent.lock()->SetScale(Vector3(0, 0, 1));
			}

			DeadPauseSelectUI();
			m_vlp_deadTimer->Start();
			m_vwp_worldSpeedManagerComponent.lock()->SetSpeed(1.0f);
		}
	}

	PauseUI();
	ScaleAnimation();

	if (m_vlp_deadTimer->Update())
	{
		Reset();
		DeadPauseWindowUI();
	}
}

void ButiEngine::PauseManagerComponent::OnSet()
{
	m_vlp_waitTimer = ObjectFactory::Create<AbsoluteTimer>(80);
	m_vlp_appearTimer = ObjectFactory::Create<AbsoluteTimer>(20);
	m_vlp_deadTimer = ObjectFactory::Create<AbsoluteTimer>(20);
}

void ButiEngine::PauseManagerComponent::OnShowUI()
{
}

void ButiEngine::PauseManagerComponent::Start()
{
	m_vlp_waitTimer->Start();
	m_vlp_appearTimer->Start();
	m_vwp_worldSpeedManagerComponent = GetManager().lock()->GetGameObject("WorldSpeedManager").lock()->GetGameComponent<WorldSpeedManager>();
	m_vwp_fadeOutComponent = GetManager().lock()->AddObjectFromCereal("FadeOutUI").lock()->GetGameComponent<FadeOutComponent>();
	m_vwp_fadeOutComponent.lock()->SetIsFade(true);
	m_vwp_fadeOutComponent.lock()->SetMoveAlpha(0.1f);
	m_isPause = false;
	Reset();
	m_defaultSelectScale = Vector3(320, 160, 1);
	m_pauseBackScale = Vector3(0, 0, 1);
	m_pauseNextSelectScale = Vector3(0, 0, 1);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::PauseManagerComponent::Clone()
{
	return ObjectFactory::Create<PauseManagerComponent>();
}

void ButiEngine::PauseManagerComponent::Reset()
{
	m_vlp_deadTimer->Stop();
	m_vlp_deadTimer->Reset();
	m_nextCount = 0;
	m_isBack = true;
	m_isNext = false;
}

void ButiEngine::PauseManagerComponent::InputSelect()
{
	if (!m_isNext) { return; }

	if (InputManager::IsTriggerUpKey() || InputManager::IsTriggerDownKey())
	{
		m_isBack = !m_isBack;
	}

	if (InputManager::IsTriggerDecideKey())
	{
		m_isNext = false;
	}
}

void ButiEngine::PauseManagerComponent::PauseUI()
{
	if (!m_isPause) { return; }

	if (m_vlp_appearTimer->Update())
	{
		if (m_nextCount == 0)
		{
			AppearPauseUI();
		}
		else if (m_nextCount == 1)
		{
			AppearSelectUI();
			m_isNext = true;
		}
		else
		{
			m_vlp_appearTimer->Stop();
		}

		m_nextCount++;
	}
}

void ButiEngine::PauseManagerComponent::AppearPauseUI()
{
	if (!m_vwp_pauseUI.lock())
	{
		m_vwp_pauseUI = GetManager().lock()->AddObjectFromCereal("PauseUI");
	}
	if (!m_vwp_pauseWindowUI.lock())
	{
		m_vwp_pauseWindowUI = GetManager().lock()->AddObjectFromCereal("PauseWindowUI");
	}

	m_vwp_expansionPauseUIComponent = m_vwp_pauseUI.lock()->GetGameComponent<ExpansionAnimationComponent>();
	m_vwp_expansionPauseUIComponent.lock()->SetScale(Vector3(775, 321, 1));
	m_vwp_expansionPauseWindowUIComponent = m_vwp_pauseWindowUI.lock()->GetGameComponent<ExpansionAnimationComponent>();
	m_vwp_expansionPauseWindowUIComponent.lock()->SetScale(Vector3(775, 520, 1));
}

void ButiEngine::PauseManagerComponent::AppearSelectUI()
{
	if (!m_vwp_pauseBackUI.lock())
	{
		m_vwp_pauseBackUI = GetManager().lock()->AddObjectFromCereal("PauseBackUI");
	}
	if (!m_vwp_pauseNextSelectUI.lock())
	{
		m_vwp_pauseNextSelectUI = GetManager().lock()->AddObjectFromCereal("PauseNextSelectUI");
	}
	if (!m_vwp_cursorUI.lock())
	{
		m_vwp_cursorUI = GetManager().lock()->AddObjectFromCereal("CursorUI");
	}

	m_vwp_cursorUI.lock()->transform->SetLocalPosition(Vector3(-180, 0, -0.5f));
}

void ButiEngine::PauseManagerComponent::DeadPauseWindowUI()
{
	if (m_vwp_pauseUI.lock())
	{
		m_vwp_pauseUI.lock()->SetIsRemove(true);
		m_vwp_pauseUI = Value_weak_ptr<GameObject>();
	}
	if (m_vwp_pauseWindowUI.lock())
	{
		m_vwp_pauseWindowUI.lock()->SetIsRemove(true);
		m_vwp_pauseWindowUI = Value_weak_ptr<GameObject>();
	}

	m_vwp_expansionPauseUIComponent = Value_weak_ptr<ExpansionAnimationComponent>();
	m_vwp_expansionPauseWindowUIComponent = Value_weak_ptr<ExpansionAnimationComponent>();
}

void ButiEngine::PauseManagerComponent::DeadPauseSelectUI()
{
	if (m_vwp_pauseBackUI.lock())
	{
		m_vwp_pauseBackUI.lock()->SetIsRemove(true);
		m_vwp_pauseBackUI = Value_weak_ptr<GameObject>();
	}
	if (m_vwp_pauseNextSelectUI.lock())
	{
		m_vwp_pauseNextSelectUI.lock()->SetIsRemove(true);
		m_vwp_pauseNextSelectUI = Value_weak_ptr<GameObject>();
	}
	if (m_vwp_cursorUI.lock())
	{
		m_vwp_cursorUI.lock()->SetIsRemove(true);
		m_vwp_cursorUI = Value_weak_ptr<GameObject>();
	}

	m_defaultSelectScale = Vector3(320, 160, 1);
	m_pauseBackScale = Vector3(0, 0, 1);
	m_pauseNextSelectScale = Vector3(0, 0, 1);
}

void ButiEngine::PauseManagerComponent::ScaleAnimation()
{
	if (!m_vwp_pauseBackUI.lock() || !m_vwp_pauseNextSelectUI.lock() || !m_vwp_cursorUI.lock()) { return; }

	Vector3 upScale = m_defaultSelectScale * 1.2f;
	if (m_isBack)
	{
		m_pauseBackScale.x = MathHelper::Lerp(m_pauseBackScale.x, upScale.x, 0.25f);
		m_pauseBackScale.y = MathHelper::Lerp(m_pauseBackScale.y, upScale.y, 0.25f);
		m_pauseNextSelectScale.x = MathHelper::Lerp(m_pauseNextSelectScale.x, m_defaultSelectScale.x, 0.25f);
		m_pauseNextSelectScale.y = MathHelper::Lerp(m_pauseNextSelectScale.y, m_defaultSelectScale.y, 0.25f);

		m_vwp_cursorUI.lock()->transform->SetLocalPositionY(0);
	}
	else
	{
		m_pauseBackScale.x = MathHelper::Lerp(m_pauseBackScale.x, m_defaultSelectScale.x, 0.25f);
		m_pauseBackScale.y = MathHelper::Lerp(m_pauseBackScale.y, m_defaultSelectScale.y, 0.25f);
		m_pauseNextSelectScale.x = MathHelper::Lerp(m_pauseNextSelectScale.x, upScale.x, 0.25f);
		m_pauseNextSelectScale.y = MathHelper::Lerp(m_pauseNextSelectScale.y, upScale.y, 0.25f);

		m_vwp_cursorUI.lock()->transform->SetLocalPositionY(-150);
	}
	m_vwp_pauseBackUI.lock()->transform->SetLocalScale(m_pauseBackScale);
	m_vwp_pauseNextSelectUI.lock()->transform->SetLocalScale(m_pauseNextSelectScale);

	m_vwp_cursorUI.lock()->transform->RollLocalRotationY_Degrees(3.0f);
}
