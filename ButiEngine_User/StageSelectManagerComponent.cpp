#include "stdafx_u.h"
#include "StageSelectManagerComponent.h"

#include "InputManager.h"

void ButiEngine::StageSelectManagerComponent::OnUpdate()
{
	if (InputManager::IsTriggerCancelKey())
	{
		NextScene();
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
}

void ButiEngine::StageSelectManagerComponent::NextScene()
{
	auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
	std::string sceneName = "GamePlay";
	sceneManager->RemoveScene(sceneName);
	sceneManager->LoadScene(sceneName);
	sceneManager->ChangeScene(sceneName);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::StageSelectManagerComponent::Clone()
{
	return ObjectFactory::Create<StageSelectManagerComponent>();
}
