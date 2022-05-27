#include "stdafx_u.h"
#include "GameOverManagerComponent.h"
#include "ShakeComponent.h"
#include "FloatMotionComponent.h"
#include "RotateAnimationComponent.h"
#include "ExpansionAnimationComponent.h"
#include "InputManager.h"
#include "WorldSpeedManager.h"
#include "SoundPlayerComponent.h"

void ButiEngine::GameOverManagerComponent::OnUpdate()
{
	InputSelect();
	if (m_vlp_waitTimer->Update())
	{
		if (m_nextCount == 0)
		{
			m_vwp_worldSpeedManagerComponent.lock()->SetSpeed(0);
			m_vlp_waitTimer->ChangeCountFrame(60);
			AppearGameOverUI();
		}
		else if (m_nextCount == 1)
		{
			AppearSelectUI();
			m_isInput = true;
		}
		else
		{
			m_vlp_waitTimer->Stop();
		}

		m_nextCount++;
	}

	ScaleAnimation();
	PlayerPikuPiku();
	SelectAnimation();
}

void ButiEngine::GameOverManagerComponent::OnSet()
{
	m_vlp_waitTimer = ObjectFactory::Create<AbsoluteTimer>(100);
	m_vlp_pikupikuTimer = ObjectFactory::Create<AbsoluteTimer>(ButiRandom::GetRandom(3, 30));
	m_vlp_selectAnimationTimer = ObjectFactory::Create<AbsoluteTimer>(10);
}

void ButiEngine::GameOverManagerComponent::OnShowUI()
{
}

void ButiEngine::GameOverManagerComponent::Start()
{
	m_vlp_waitTimer->Start();
	m_vlp_pikupikuTimer->Start();
	GetManager().lock()->AddObjectFromCereal("FadeOutUI");
	m_vwp_selectFlashEffectUI[0] = GetManager().lock()->AddObjectFromCereal("SelectFlashEffect");
	m_vwp_selectFlashEffectUI[1] = GetManager().lock()->AddObjectFromCereal("SelectFlashEffect");
	m_vwp_worldSpeedManagerComponent = GetManager().lock()->GetGameObject("WorldSpeedManager").lock()->GetGameComponent<WorldSpeedManager>();
	m_vwp_soundPlayerComponent = GetManager().lock()->GetGameObject("SoundPlayer").lock()->GetGameComponent<SoundPlayerComponent>();
	m_isRetry = true;
	m_isNext = false;
	m_isInput = false;
	m_isSelectAnimation = false;
	m_nextCount = 0;
	m_selectAnimationStep = 0;
	m_selectAnimationScale = 0;
	m_selectAnimationRotate = 0;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GameOverManagerComponent::Clone()
{
	return ObjectFactory::Create<GameOverManagerComponent>();
}

void ButiEngine::GameOverManagerComponent::AppearGameOverUI()
{
	auto gameOverUI = GetManager().lock()->AddObjectFromCereal("GameOverUI");
	auto floatMotion_gameOverUI = gameOverUI.lock()->GetGameComponent<FloatMotionComponent>();
	floatMotion_gameOverUI->SetAmplitude(15.0f);
	floatMotion_gameOverUI->SetMotionSpeed(0.03f);
	floatMotion_gameOverUI->SetIsRandomSpeed(false);
	floatMotion_gameOverUI->SetIsAbsolute(true);
	auto rotateAnimation_gameOverUI = gameOverUI.lock()->GetGameComponent<RotateAnimationComponent>();
	rotateAnimation_gameOverUI->SetMotionSpeed(0.05f);
	rotateAnimation_gameOverUI->SetIsRandomSpeed(false);
	rotateAnimation_gameOverUI->SetAxisType(2);
	rotateAnimation_gameOverUI->SetIsAbsolute(true);
	auto expansion_gameOverUI = gameOverUI.lock()->GetGameComponent<ExpansionAnimationComponent>();
	expansion_gameOverUI->SetScale(Vector3(1544, 360, 1));

	m_vwp_gameOverPlayerUI = GetManager().lock()->AddObjectFromCereal("GameOverPlayerUI");
	m_vwp_gameOverPlayerUI.lock()->GetGameComponent<ShakeComponent>()->SetIsAbsolute(true);
	auto expansion_gameOverPlayerUI = m_vwp_gameOverPlayerUI.lock()->GetGameComponent<ExpansionAnimationComponent>();
	expansion_gameOverPlayerUI->SetScale(Vector3(8, 8, 8));
}

void ButiEngine::GameOverManagerComponent::AppearSelectUI()
{
	m_vwp_cursorUI = GetManager().lock()->AddObjectFromCereal("CursorUI");
	m_vwp_cursorUI.lock()->transform->SetLocalPosition(Vector3(-625, -130, -0.1f));
	auto floatMotion_cursorUI = m_vwp_cursorUI.lock()->GetGameComponent<FloatMotionComponent>();
	floatMotion_cursorUI->SetAmplitude(10.0f);
	floatMotion_cursorUI->SetMotionSpeed(0.1f);
	floatMotion_cursorUI->SetIsRandomSpeed(false);
	floatMotion_cursorUI->SetIsAbsolute(true);

	m_vwp_retryUI = GetManager().lock()->AddObjectFromCereal("RetryUI");
	m_vwp_nextTitleUI = GetManager().lock()->AddObjectFromCereal("NextTitleUI");

	m_defaultSelectScale = Vector3(327, 139, 1);
	m_retryScale = Vector3(0, 0, 1);
	m_nextTitleScale = Vector3(0, 0, 1);
}

void ButiEngine::GameOverManagerComponent::InputSelect()
{
	if (!m_isInput) { return; }

	if (InputManager::IsTriggerLeftKey() || InputManager::IsTriggerRightKey())
	{
		m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/UI_Select.wav"));
		m_isRetry = !m_isRetry;
	}

	if (InputManager::IsTriggerDecideKey())
	{
		m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/UI_Enter.wav"));
		m_isInput = false;
		m_isSelectAnimation = true;
		m_vlp_selectAnimationTimer->Start();
	}
}

void ButiEngine::GameOverManagerComponent::ScaleAnimation()
{
	if (!m_vwp_retryUI.lock() || !m_vwp_nextTitleUI.lock() || !m_vwp_cursorUI.lock()) { return; }

	Vector3 upScale = m_defaultSelectScale * 1.2f;
	if (m_isRetry)
	{
		m_retryScale.x = MathHelper::Lerp(m_retryScale.x, upScale.x, 0.25f);
		m_retryScale.y = MathHelper::Lerp(m_retryScale.y, upScale.y, 0.25f);
		m_nextTitleScale.x = MathHelper::Lerp(m_nextTitleScale.x, m_defaultSelectScale.x, 0.25f);
		m_nextTitleScale.y = MathHelper::Lerp(m_nextTitleScale.y, m_defaultSelectScale.y, 0.25f);
		
		m_vwp_cursorUI.lock()->transform->SetLocalPositionX(-625);
	}
	else
	{
		m_retryScale.x = MathHelper::Lerp(m_retryScale.x, m_defaultSelectScale.x, 0.25f);
		m_retryScale.y = MathHelper::Lerp(m_retryScale.y, m_defaultSelectScale.y, 0.25f);
		m_nextTitleScale.x = MathHelper::Lerp(m_nextTitleScale.x, upScale.x, 0.25f);
		m_nextTitleScale.y = MathHelper::Lerp(m_nextTitleScale.y, upScale.y, 0.25f);

		m_vwp_cursorUI.lock()->transform->SetLocalPositionX(185);
	}
	m_vwp_retryUI.lock()->transform->SetLocalScale(m_retryScale);
	m_vwp_nextTitleUI.lock()->transform->SetLocalScale(m_nextTitleScale);

	m_vwp_cursorUI.lock()->transform->RollLocalRotationY_Degrees(3.0f);
}

void ButiEngine::GameOverManagerComponent::PlayerPikuPiku()
{
	if (!m_vwp_gameOverPlayerUI.lock()) { return; }

	if (m_vlp_pikupikuTimer->Update())
	{
		m_vlp_pikupikuTimer->ChangeCountFrame(ButiRandom::GetRandom(20, 40));
		m_vwp_gameOverPlayerUI.lock()->GetGameComponent<ShakeComponent>()->Shake(0.5f, 2);
	}
	
}

void ButiEngine::GameOverManagerComponent::SelectAnimation()
{
	if (!m_isSelectAnimation) { return; }

	if (m_vlp_selectAnimationTimer->Update())
	{
		if (m_selectAnimationStep == 0)
		{
			//セレクトフラッシュを生成
			m_vwp_selectFlashEffectUI[0] = GetManager().lock()->AddObjectFromCereal("SelectFlashEffect");
			m_vwp_selectFlashEffectUI[1] = GetManager().lock()->AddObjectFromCereal("SelectFlashEffect");
			Vector3 position;
			if (m_isRetry)
			{
				position = Vector3(-410, -130, -10);
			}
			else
			{
				position = Vector3(410, -130, -10);
			}
			m_vwp_selectFlashEffectUI[0].lock()->transform->SetLocalPosition(position);
			m_vwp_selectFlashEffectUI[1].lock()->transform->SetLocalPosition(position);
			m_vwp_selectFlashEffectUI[1].lock()->transform->SetLocalRotationZ_Degrees(90);

			m_selectAnimationRotate = 16;

			m_vlp_selectAnimationTimer->ChangeCountFrame(8);
		}
		else if (m_selectAnimationStep == 1)
		{
			m_vlp_selectAnimationTimer->ChangeCountFrame(25);
		}
		else if(m_selectAnimationStep == 2)
		{
			m_isNext = true;
		}

		m_selectAnimationStep++;
	}

	//スケールを変える（小→大→小）
	if (m_selectAnimationStep == 1)
	{
		m_selectAnimationScale = MathHelper::Lerp(m_selectAnimationScale, 200.0f, 0.6f);
		m_vwp_selectFlashEffectUI[0].lock()->transform->RollLocalRotationZ_Degrees(-1);
		m_vwp_selectFlashEffectUI[1].lock()->transform->RollLocalRotationZ_Degrees(-1);
	}
	else if (m_selectAnimationStep == 2)
	{
		m_selectAnimationScale = MathHelper::Lerp(m_selectAnimationScale, 0.0f, 0.2f);
		m_selectAnimationRotate = MathHelper::Lerp(m_selectAnimationRotate, 1.0f, 0.08f);
		m_vwp_selectFlashEffectUI[0].lock()->transform->RollLocalRotationZ_Degrees(-m_selectAnimationRotate);
		m_vwp_selectFlashEffectUI[1].lock()->transform->RollLocalRotationZ_Degrees(-m_selectAnimationRotate);
	}
	else
	{
		m_selectAnimationScale = 0;
	}

	//スケールと回転の更新
	if (m_vwp_selectFlashEffectUI[0].lock())
	{
		auto scale = m_vwp_selectFlashEffectUI[0].lock()->transform->GetLocalScale();
		m_vwp_selectFlashEffectUI[0].lock()->transform->SetLocalScale(Vector3(scale.x, m_selectAnimationScale, scale.z));
		m_vwp_selectFlashEffectUI[1].lock()->transform->SetLocalScale(Vector3(scale.x, m_selectAnimationScale, scale.z));
	}
}
