#include "stdafx_u.h"
#include "StageSelectManagerComponent.h"
#include "SceneChangeAnimationComponent.h"
#include "InputManager.h"
#include "FloatMotionComponent.h"
#include "SoundPlayerComponent.h"
#include "ButiEngineHeader/Header/GameObjects/DefaultGameComponent/SpriteAnimationComponent.h"

void ButiEngine::StageSelectManagerComponent::OnUpdate()
{
	if (!isSceneChange)
	{
		isSceneChange = true;
		m_vwp_gamePlayChangeAnimation.lock()->SceneStart();
	}
	if (!m_waitTimer->Update_continue()) { return; }

	if (InputManager::IsTriggerRightKey())
	{
		m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/UI_Select.wav"));
		m_stageNum++;
	}
	else if(InputManager::IsTriggerLeftKey())
	{
		m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/UI_Select.wav"));
		m_stageNum--;
	}

	FixStageNum();

	if (InputManager::IsTriggerDecideKey())
	{
		m_vwp_gamePlayChangeAnimation.lock()->SceneEnd();
		isSceneChange = false;
		m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/UI_Enter.wav"));

	}

	if (!isSceneChange && !m_vwp_gamePlayChangeAnimation.lock()->IsAnimation()) 
	{
		//次のシーンへ
		NextScene();
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->StopBGM();
	}
}

void ButiEngine::StageSelectManagerComponent::OnSet()
{
	m_waitTimer = ObjectFactory::Create<AbsoluteTimer>(60);
}

void ButiEngine::StageSelectManagerComponent::OnShowUI()
{
}

void ButiEngine::StageSelectManagerComponent::Start()
{
	m_waitTimer->Start();
	m_stageNum = 0;
	m_maxStageNum = 1;
	isSceneChange = false;

	auto sceneChangeAnimation = GetManager().lock()->AddObjectFromCereal("SceneChangeAnimation");
	m_vwp_gamePlayChangeAnimation = sceneChangeAnimation.lock()->GetGameComponent<SceneChangeAnimationComponent>();
	m_vwp_soundPlayerComponent = GetManager().lock()->GetGameObject("SoundPlayer").lock()->GetGameComponent<SoundPlayerComponent>();
	
	m_vwp_soundPlayerComponent.lock()->PlayBGM(SoundTag("Sound/BGM2.wav"));
	
}

void ButiEngine::StageSelectManagerComponent::End()
{
}

void ButiEngine::StageSelectManagerComponent::NextScene()
{
	auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
	//std::string sceneName = "GamePlay";
	std::string sceneName = "Stage_" + std::to_string(m_stageNum);
	GUI::Console("Stage_" + std::to_string(m_stageNum)+"への移動");
	sceneManager->RemoveScene(sceneName);
	sceneManager->LoadScene(sceneName);
	sceneManager->ChangeScene(sceneName);
}

void ButiEngine::StageSelectManagerComponent::BackScene()
{
	auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
	std::string sceneName = "StageSelect";
	sceneManager->RemoveScene(sceneName);
	sceneManager->LoadScene(sceneName);
	sceneManager->ChangeScene(sceneName);
}

std::int16_t ButiEngine::StageSelectManagerComponent::GetStageNum()
{
	return m_stageNum;
}

void ButiEngine::StageSelectManagerComponent::FixStageNum()
{
	//ステージ番号を範囲内に修正
	if (m_stageNum > m_maxStageNum)
	{
		m_stageNum = 0;
	}
	else if(m_stageNum < 0)
	{
		m_stageNum = m_maxStageNum;
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::StageSelectManagerComponent::Clone()
{
	return ObjectFactory::Create<StageSelectManagerComponent>();
}
