#include "stdafx_u.h"
#include "TitleManagerComponent.h"

#include "InputManager.h"

void ButiEngine::TitleManagerComponent::OnUpdate()
{
	if (InputManager::IsTriggerDecideKey())
	{
		//ŽŸ‚ÌƒV[ƒ“‚Ö
		NextScene();
	}
}

void ButiEngine::TitleManagerComponent::OnSet()
{
}

void ButiEngine::TitleManagerComponent::OnShowUI()
{
}

void ButiEngine::TitleManagerComponent::Start()
{
}

void ButiEngine::TitleManagerComponent::NextScene()
{
	auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
	std::string sceneName = "StageSelect";
	sceneManager->RemoveScene(sceneName);
	sceneManager->LoadScene(sceneName);
	sceneManager->ChangeScene(sceneName);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::TitleManagerComponent::Clone()
{
	return ObjectFactory::Create<TitleManagerComponent>();
}
