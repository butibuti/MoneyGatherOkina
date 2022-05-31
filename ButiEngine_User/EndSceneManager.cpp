#include "stdafx_u.h"
#include "EndSceneManager.h"
#include "InputManager.h"
#include "CrossBeamEffectComponent.h"
#include "SoundPlayerComponent.h"
#include "MoveAnimationComponent.h"
#include "StageSelectManagerComponent.h"

void ButiEngine::EndSceneManager::OnUpdate()
{
	Animation();

	if (!m_vlp_waitTimer->Update_continue()) { return; }

	if (!m_isOnce)
	{
		m_isOnce = true;
		m_vwp_buttonUI = GetManager().lock()->AddObjectFromCereal("BackToSelectUI");
		auto position = m_vwp_buttonUI.lock()->transform->GetLocalPosition();
		auto moveAnimationComponent = m_vwp_buttonUI.lock()->GetGameComponent<MoveAnimationComponent>();
		auto endPosition = Vector3(0, 0, position.z);
		moveAnimationComponent->SetEndPosition(endPosition);
		moveAnimationComponent->SetSpeed(1.0f);
		moveAnimationComponent->SetIsShake(true);
		moveAnimationComponent->SetIsActive(true);

		m_vwp_soundPlayerComponent.lock()->PlayIsolateSE(SoundTag("Sound/Beam.wav"));
	}

	m_buttonScale.x = MathHelper::Lerp(m_buttonScale.x, m_endScale.x, 0.2f);
	m_buttonScale.y = MathHelper::Lerp(m_buttonScale.y, m_endScale.y, 0.2f);

	if (m_vwp_buttonUI.lock())
	{
		m_vwp_buttonUI.lock()->transform->SetLocalScale(m_buttonScale);
	}

	if (InputManager::IsTriggerDecideKey())
	{
		if (!m_isNext)
		{
			//クロスフラッシュ生成
			m_isNext = true;
			Vector3 position = Vector3(0, 0, -10);
			auto crossBeamEffect = GetManager().lock()->AddObjectFromCereal("CrossBeamEffect").lock()->GetGameComponent<CrossBeamEffectComponent>();
			crossBeamEffect->SetPosition(position);
			m_vwp_soundPlayerComponent.lock()->PlayIsolateSE(SoundTag("Sound/Beam.wav"));

			m_endScale = Vector3(0, 0, 0);
		}

	}

	if (m_isNext)
	{
		if (m_vlp_nextTimer->Update())
		{
			StageSelectManagerComponent::SetStageNum(0);

			auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
			std::string sceneName = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneInformation()->GetSceneName();
			sceneManager->RemoveScene(sceneName);
			sceneName = "StageSelect";
			sceneManager->LoadScene(sceneName);
			sceneManager->ChangeScene(sceneName);
		}
	}

}

void ButiEngine::EndSceneManager::OnSet()
{
	m_vlp_waitTimer = ObjectFactory::Create<AbsoluteTimer>(1100);
	m_vlp_nextTimer = ObjectFactory::Create<AbsoluteTimer>(50);
}

void ButiEngine::EndSceneManager::OnShowUI()
{
}

void ButiEngine::EndSceneManager::Start()
{
	m_vlp_waitTimer->Start();
	m_vlp_nextTimer->Start();
	m_vlp_waitTimer->Reset();
	m_vlp_nextTimer->Reset();

	m_vwp_soundPlayerComponent = GetManager().lock()->GetGameObject("SoundPlayer").lock()->GetGameComponent<SoundPlayerComponent>();
	m_vwp_endRollUI = GetManager().lock()->GetGameObject("EndRollUI");
	m_vwp_endRollUI.lock()->transform->SetLocalPosition(Vector3(0, -1170, -1.0f));

	m_buttonScale = Vector3(544, 202, 0);
	m_endScale = m_buttonScale;

	m_isNext = false;
	m_isOnce = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::EndSceneManager::Clone()
{
	return ObjectFactory::Create<EndSceneManager>();
}

void ButiEngine::EndSceneManager::Animation()
{
	m_vwp_endRollUI.lock()->transform->TranslateY(2.16f);
}
