#include "stdafx_u.h"
#include "GameOverManagerComponent.h"
#include "ShakeComponent.h"
#include "FloatMotionComponent.h"
#include "RotateAnimationComponent.h"
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

	ScaleAnimation();
	PlayerPikuPiku();
}

void ButiEngine::GameOverManagerComponent::OnSet()
{
	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>();
	m_vlp_pikupikuTimer = ObjectFactory::Create<RelativeTimer>();
}

void ButiEngine::GameOverManagerComponent::OnShowUI()
{
}

void ButiEngine::GameOverManagerComponent::Start()
{
	m_vlp_waitTimer->Start();
	m_vlp_waitTimer->ChangeCountFrame(120);
	m_vlp_pikupikuTimer->Start();
	m_vlp_pikupikuTimer->ChangeCountFrame(ButiRandom::GetRandom(3, 30));
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
	auto gameOverUI = GetManager().lock()->AddObjectFromCereal("GameOverUI");
	auto floatMotion_gameOverUI = gameOverUI.lock()->GetGameComponent<FloatMotionComponent>();
	floatMotion_gameOverUI->SetAmplitude(15.0f);
	floatMotion_gameOverUI->SetMotionSpeed(0.03f);
	floatMotion_gameOverUI->SetIsRandomSpeed(false);
	auto rotateAnimation_gameOverUI = gameOverUI.lock()->GetGameComponent<RotateAnimationComponent>();
	rotateAnimation_gameOverUI->SetMotionSpeed(0.05f);
	rotateAnimation_gameOverUI->SetIsRandomSpeed(false);
	rotateAnimation_gameOverUI->SetAxisType(2);

	m_vwp_cursorUI = GetManager().lock()->AddObjectFromCereal("CursorUI");
	m_vwp_cursorUI.lock()->transform->SetLocalPosition(Vector3(-625, -130, -0.1f));
	auto floatMotion_cursorUI = m_vwp_cursorUI.lock()->GetGameComponent<FloatMotionComponent>();
	floatMotion_cursorUI->SetAmplitude(10.0f);
	floatMotion_cursorUI->SetMotionSpeed(0.1f);
	floatMotion_cursorUI->SetIsRandomSpeed(false);

	m_vwp_gameOverPlayerUI = GetManager().lock()->AddObjectFromCereal("GameOverPlayerUI");
	m_vwp_retryUI = GetManager().lock()->AddObjectFromCereal("RetryUI");
	m_vwp_nextTitleUI = GetManager().lock()->AddObjectFromCereal("NextTitleUI");

	m_defaultScale = m_vwp_retryUI.lock()->transform->GetLocalScale();
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

void ButiEngine::GameOverManagerComponent::ScaleAnimation()
{
	if (!m_vwp_retryUI.lock() || !m_vwp_nextTitleUI.lock() || !m_vwp_cursorUI.lock()) { return; }

	Vector3 upScale = m_defaultScale * 1.2f;
	if (m_isRetry)
	{
		m_vwp_retryUI.lock()->transform->SetLocalScale(upScale);
		m_vwp_nextTitleUI.lock()->transform->SetLocalScale(m_defaultScale);
		m_vwp_cursorUI.lock()->transform->SetLocalPositionX(-625);
	}
	else
	{
		m_vwp_retryUI.lock()->transform->SetLocalScale(m_defaultScale);
		m_vwp_nextTitleUI.lock()->transform->SetLocalScale(upScale);
		m_vwp_cursorUI.lock()->transform->SetLocalPositionX(185);
	}

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
