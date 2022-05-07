#include "stdafx_u.h"
#include "StageSelectManagerComponent.h"

#include "InputManager.h"

void ButiEngine::StageSelectManagerComponent::OnUpdate()
{
	if (InputManager::IsTriggerRightKey())
	{
		m_stageNum++;
	}
	else if(InputManager::IsTriggerLeftKey())
	{
		m_stageNum--;
	}

	FixStageNum();

	if (InputManager::IsTriggerDecideKey())
	{
		//次のシーンへ
		NextScene();
	}
	else if (InputManager::IsTriggerCancelKey())
	{
		//タイトルに戻る
		BackScene();
	}
}

void ButiEngine::StageSelectManagerComponent::OnSet()
{
}

void ButiEngine::StageSelectManagerComponent::OnShowUI()
{
}

void ButiEngine::StageSelectManagerComponent::Start()
{
	m_stageNum = 0;
	m_maxStageNum = 3;
}

void ButiEngine::StageSelectManagerComponent::NextScene()
{
	auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
	std::string sceneName = "GamePlay";
	//std::string sceneName = "Stage_" + std::to_string(m_stageNum);
	sceneManager->RemoveScene(sceneName);
	sceneManager->LoadScene(sceneName);
	sceneManager->ChangeScene(sceneName);
}

void ButiEngine::StageSelectManagerComponent::BackScene()
{
	auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
	std::string sceneName = "Title";
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
