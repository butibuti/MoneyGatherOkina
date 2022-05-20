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
	auto floatMotion = gameOverUI.lock()->GetGameComponent<FloatMotionComponent>();
	floatMotion->SetAmplitude(15.0f);
	floatMotion->SetMotionSpeed(0.03f);
	floatMotion->SetIsRandomSpeed(false);
	auto rotateAnimation = gameOverUI.lock()->GetGameComponent<RotateAnimationComponent>();
	rotateAnimation->SetMotionSpeed(0.05f);
	rotateAnimation->SetIsRandomSpeed(false);
	rotateAnimation->SetAxisType(2);

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
	if (!m_vwp_retryUI.lock() || !m_vwp_nextTitleUI.lock()) { return; }

	Vector3 upScale = m_defaultScale * 1.2f;
	if (m_isRetry)
	{
		m_vwp_retryUI.lock()->transform->SetLocalScale(upScale);
		m_vwp_nextTitleUI.lock()->transform->SetLocalScale(m_defaultScale);
	}
	else
	{
		m_vwp_retryUI.lock()->transform->SetLocalScale(m_defaultScale);
		m_vwp_nextTitleUI.lock()->transform->SetLocalScale(upScale);
	}
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
