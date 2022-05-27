#include "stdafx_u.h"
#include "PauseManagerComponent.h"
#include "WorldSpeedManager.h"
#include "ExpansionAnimationComponent.h"
#include "InputManager.h"
#include "FadeOutComponent.h"
#include "WaveManager.h"
#include "FloatMotionComponent.h"
#include "SoundPlayerComponent.h"
#include "CameraShakeComponent.h"

void ButiEngine::PauseManagerComponent::OnUpdate()
{
	//クリアかゲームオーバー中はポーズできないようにする
	if (m_vwp_waveManagerComponent.lock()->IsGameOver() ||
		m_vwp_waveManagerComponent.lock()->IsClearAnimation()) 
	{
		return;
	}

	//入力の受付
	InputSelect();

	//ポーズボタンを押したとき
	if (m_vlp_waitTimer->Update_continue() && InputManager::IsTriggerPauseKey())
	{
		//タイマー関連を一度リセット
		Reset();
		m_vlp_waitTimer->Reset();
		m_vlp_appearTimer->Reset();
		if (m_isPause)
		{
			m_isPauseExit = true;
			//ポーズを解除する
			DeadPauseSelectUI();
			m_vlp_deadTimer->Start();
			m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/UI_Cansel.wav"));
		}
		else
		{
			m_isPause = true;
		}

		//現在がポーズ中かどうかで処理を変える
		if (m_isPause && !m_isPauseExit)
		{
			//ポーズ中にする
			m_vlp_appearTimer->Start();
			m_vwp_fadeOutComponent.lock()->SetIsFade(false);
			m_vwp_worldSpeedManagerComponent.lock()->SetSpeed(0.0f);
			m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/UI_Enter.wav"));
			InputManager::VibrationStop();
			GetManager().lock()->GetGameObject("Camera").lock()->GetGameComponent<CameraShakeComponent>()->ShakeStop();
		}
	}

	//ポーズ画面のUIを順々に追加していく処理がまとめられた関数
	PauseUI();

	ScaleAnimation();
	ExitUpdate();

	//ポーズ画面を閉じた際にフェードアウトやUIを引っ込めたり色々する場所
	if (m_vlp_deadTimer->Update())
	{
		Reset();
		m_vwp_fadeOutComponent.lock()->SetIsFade(true);
		DeadPauseWindowUI();
	}
}

void ButiEngine::PauseManagerComponent::OnSet()
{
	m_vlp_waitTimer = ObjectFactory::Create<AbsoluteTimer>(40);
	m_vlp_appearTimer = ObjectFactory::Create<AbsoluteTimer>(20);
	m_vlp_deadTimer = ObjectFactory::Create<AbsoluteTimer>(20);
	m_vlp_exitTimer = ObjectFactory::Create<AbsoluteTimer>(20);
}

void ButiEngine::PauseManagerComponent::OnShowUI()
{
}

void ButiEngine::PauseManagerComponent::Start()
{
	m_vlp_waitTimer->Start();
	m_vlp_appearTimer->Start();
	m_vlp_exitTimer->Start();

	m_vwp_worldSpeedManagerComponent = GetManager().lock()->GetGameObject("WorldSpeedManager").lock()->GetGameComponent<WorldSpeedManager>();
	m_vwp_waveManagerComponent = GetManager().lock()->GetGameObject("WaveManager").lock()->GetGameComponent<WaveManager>();
	
	m_vwp_fadeOutComponent = GetManager().lock()->AddObjectFromCereal("FadeOutUI").lock()->GetGameComponent<FadeOutComponent>();
	m_vwp_fadeOutComponent.lock()->SetIsFade(true);
	m_vwp_fadeOutComponent.lock()->SetMoveAlpha(0.1f);
	m_vwp_fadeOutComponent.lock()->SetPositionZ(-0.051f);
	m_vwp_fadeOutComponent.lock()->SetMaxAlpha(0.8f);

	m_vwp_pauseUI = GetManager().lock()->AddObjectFromCereal("PauseUI");
	m_vwp_pauseWindowUI = GetManager().lock()->AddObjectFromCereal("PauseWindowUI");

	m_vwp_expansionPauseUIComponent = m_vwp_pauseUI.lock()->GetGameComponent<ExpansionAnimationComponent>();
	m_vwp_expansionPauseUIComponent.lock()->SetIsActive(false);
	m_vwp_expansionPauseUIComponent.lock()->SetScale(Vector3(775, 321, 1));
	m_vwp_expansionPauseWindowUIComponent = m_vwp_pauseWindowUI.lock()->GetGameComponent<ExpansionAnimationComponent>();
	m_vwp_expansionPauseWindowUIComponent.lock()->SetIsActive(false);
	m_vwp_expansionPauseWindowUIComponent.lock()->SetScale(Vector3(775, 520, 1));

	m_vwp_pauseBackUI = GetManager().lock()->AddObjectFromCereal("PauseBackUI");
	m_vwp_pauseNextSelectUI = GetManager().lock()->AddObjectFromCereal("PauseNextSelectUI");
	m_vwp_cursorUI = GetManager().lock()->AddObjectFromCereal("CursorUI");
	m_vwp_cursorUI.lock()->transform->SetLocalPosition(Vector3(-180, 0, -0.5f));
	m_vwp_cursorUI.lock()->transform->SetLocalScale(Vector3(0, 0, 0));
	auto floatMotion_cursorUI = m_vwp_cursorUI.lock()->GetGameComponent<FloatMotionComponent>();
	floatMotion_cursorUI->SetIsRemove(true);

	m_vwp_soundPlayerComponent = GetManager().lock()->GetGameObject("SoundPlayer").lock()->GetGameComponent<SoundPlayerComponent>();

	m_isPause = false;
	m_isPauseExit = false;
	Reset();
	m_defaultSelectScale = Vector3(320, 160, 1);
	m_currentSelectScale = Vector3(0, 0, 1);
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
		m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/UI_Select.wav"));
		m_isBack = !m_isBack;
	}

	if (InputManager::IsTriggerDecideKey())
	{
		m_isNext = false;
		if (m_isBack)
		{
			m_isPauseExit = true;

			//ポーズを解除する
			DeadPauseSelectUI();
			m_vlp_deadTimer->Start();
			m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/UI_Cansel.wav"));
		}
		else
		{
			m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/UI_Enter.wav"));
		}
		m_vlp_waitTimer->Reset();
		m_vlp_appearTimer->Reset();
	}
}

void ButiEngine::PauseManagerComponent::PauseUI()
{
	//ポーズが解除されていたら通らない
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
	if (!m_vwp_expansionPauseUIComponent.lock() || !m_vwp_expansionPauseWindowUIComponent.lock()) { return; }
	m_vwp_expansionPauseUIComponent.lock()->SetIsActive(true);
	m_vwp_expansionPauseWindowUIComponent.lock()->SetIsActive(true);
	m_vwp_expansionPauseUIComponent.lock()->SetScale(Vector3(775, 321, 1));
	m_vwp_expansionPauseWindowUIComponent.lock()->SetScale(Vector3(775, 520, 1));
}

void ButiEngine::PauseManagerComponent::AppearSelectUI()
{
	m_currentSelectScale = m_defaultSelectScale;
	m_vwp_cursorUI.lock()->transform->SetLocalScale(Vector3(60, 10, 60));
}

void ButiEngine::PauseManagerComponent::DeadPauseWindowUI()
{
	if (!m_vwp_expansionPauseUIComponent.lock() || !m_vwp_expansionPauseWindowUIComponent.lock()) { return; }
	m_vwp_expansionPauseUIComponent.lock()->SetIsActive(true);
	m_vwp_expansionPauseWindowUIComponent.lock()->SetIsActive(true);
	m_vwp_expansionPauseUIComponent.lock()->SetScale(Vector3(0, 0, 1));
	m_vwp_expansionPauseWindowUIComponent.lock()->SetScale(Vector3(0, 0, 1));
}

void ButiEngine::PauseManagerComponent::DeadPauseSelectUI()
{
	m_currentSelectScale = Vector3(0, 0, 0);
	m_vwp_cursorUI.lock()->transform->SetLocalScale(Vector3(0, 0, 0));
}

void ButiEngine::PauseManagerComponent::ScaleAnimation()
{
	if (!m_vwp_pauseBackUI.lock() || !m_vwp_pauseNextSelectUI.lock() || !m_vwp_cursorUI.lock()) { return; }

	Vector3 upScale = m_currentSelectScale * 1.2f;
	if (m_isBack)
	{
		m_pauseBackScale.x = MathHelper::Lerp(m_pauseBackScale.x, upScale.x, 0.25f);
		m_pauseBackScale.y = MathHelper::Lerp(m_pauseBackScale.y, upScale.y, 0.25f);
		m_pauseNextSelectScale.x = MathHelper::Lerp(m_pauseNextSelectScale.x, m_currentSelectScale.x, 0.25f);
		m_pauseNextSelectScale.y = MathHelper::Lerp(m_pauseNextSelectScale.y, m_currentSelectScale.y, 0.25f);

		m_vwp_cursorUI.lock()->transform->SetLocalPosition(Vector3(-180, 0, -0.5f));
	}
	else
	{
		m_pauseBackScale.x = MathHelper::Lerp(m_pauseBackScale.x, m_currentSelectScale.x, 0.25f);
		m_pauseBackScale.y = MathHelper::Lerp(m_pauseBackScale.y, m_currentSelectScale.y, 0.25f);
		m_pauseNextSelectScale.x = MathHelper::Lerp(m_pauseNextSelectScale.x, upScale.x, 0.25f);
		m_pauseNextSelectScale.y = MathHelper::Lerp(m_pauseNextSelectScale.y, upScale.y, 0.25f);

		m_vwp_cursorUI.lock()->transform->SetLocalPosition(Vector3(-180, -150, -0.5f));
	}
	m_vwp_pauseBackUI.lock()->transform->SetLocalScale(m_pauseBackScale);
	m_vwp_pauseNextSelectUI.lock()->transform->SetLocalScale(m_pauseNextSelectScale);

	m_vwp_cursorUI.lock()->transform->RollLocalRotationY_Degrees(3.0f);
}

void ButiEngine::PauseManagerComponent::ExitUpdate()
{
	if (!m_isPause || !m_isPauseExit) { return; }

	if (m_vlp_exitTimer->Update())
	{
		m_isPauseExit = false;
		m_isPause = false;
		m_vlp_exitTimer->Reset();

		m_vwp_worldSpeedManagerComponent.lock()->SetSpeed(1.0f);
	}
}
