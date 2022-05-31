#include "stdafx_u.h"
#include "EndSceneManager.h"
#include "InputManager.h"
#include "CrossBeamEffectComponent.h"
#include "SoundPlayerComponent.h"
#include "MoveAnimationComponent.h"

void ButiEngine::EndSceneManager::OnUpdate()
{
	Animation();

	if (!m_vlp_waitTimer->Update_continue()) { return; }

	if (!m_isOnce)
	{
		m_isOnce = true;
		auto backToSelectUI = GetManager().lock()->AddObjectFromCereal("BackToSelectUI");
		auto position = backToSelectUI.lock()->transform->GetLocalPosition();
		auto moveAnimationComponent = backToSelectUI.lock()->GetGameComponent<MoveAnimationComponent>();
		auto endPosition = Vector3(0, 0, position.z);
		moveAnimationComponent->SetEndPosition(endPosition);
		moveAnimationComponent->SetSpeed(1.0f);
		moveAnimationComponent->SetIsShake(true);
		moveAnimationComponent->SetIsActive(true);

		m_vwp_soundPlayerComponent.lock()->PlayIsolateSE(SoundTag("Sound/Beam.wav"));
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
		}

	}

	if (m_isNext)
	{
		if (m_vlp_nextTimer->Update())
		{
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
	m_vlp_waitTimer = ObjectFactory::Create<AbsoluteTimer>(720);
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

	m_isNext = false;
	m_isOnce = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::EndSceneManager::Clone()
{
	return ObjectFactory::Create<EndSceneManager>();
}

void ButiEngine::EndSceneManager::Animation()
{
	m_vwp_endRollUI.lock()->transform->TranslateY(3.6f);
}
