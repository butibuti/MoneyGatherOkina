#include "stdafx_u.h"
#include "StageSelectManagerComponent.h"
#include "SceneChangeAnimationComponent.h"
#include"TitleManagerComponent.h"
#include "InputManager.h"
#include"FloatMotionComponent.h"
#include "Flocking.h"
#include"ButiEngineHeader/Header/GameObjects/DefaultGameComponent/SpriteAnimationComponent.h"
void ButiEngine::StageSelectManagerComponent::OnUpdate()
{
	if (m_vwp_title.lock()->IsTitle()) {
		return;
	}

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
		m_vwp_gamePlayChangeAnimation.lock()->SceneEnd();
		isSceneChange = true;
	}
	else if (InputManager::IsTriggerCancelKey())
	{
		//タイトルに戻る
		BackScene();
	}

	if (isSceneChange&&!m_vwp_gamePlayChangeAnimation.lock()->IsAnimation()) {
		isSceneChange = false;
		//次のシーンへ
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
	m_stageNum = 1;
	m_maxStageNum = 4;
	if (!m_vwp_gamePlayChangeAnimation.lock()) {
		m_vwp_gamePlayChangeAnimation = gameObject.lock()->GetGameComponent<SceneChangeAnimationComponent>();
		m_vwp_gamePlayChangeAnimation.lock()->HidePanel();
		auto playerFloatAnim = GetManager().lock()->GetGameObject("DrawObject_Player").lock()->GetGameComponent<FloatMotionComponent>();
		playerFloatAnim->SetAmplitude(0.1f);
		playerFloatAnim->SetMotionSpeed(0.035f);
		auto lastBossFloatAnim = GetManager().lock()->GetGameObject("DrawObject_LastBoss").lock()->GetGameComponent<FloatMotionComponent>();
		lastBossFloatAnim->SetAmplitude(0.32f);
		lastBossFloatAnim->SetMotionSpeed(0.015f);
		for (std::int32_t i = 0; i < 4; i++) {
			auto workerFloatAnim = GetManager().lock()->GetGameObject("DrawObject_Worker_"+std::to_string(i)).lock()->GetGameComponent<FloatMotionComponent>();
			workerFloatAnim->SetMinTargetSpeed(0.05f);
			workerFloatAnim->SetMaxTargetSpeed(0.2f);
		}
		

	}
	else {
		m_vwp_gamePlayChangeAnimation.lock()->SceneStart();
	}
	if (!m_vwp_title.lock()) {
		m_vwp_title = gameObject.lock()->GetGameComponent<TitleManagerComponent>();
	}
	GetCamera("BloomSource")->vlp_transform->SetBaseTransform(GetCamera("main")->vlp_transform,true);
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
		m_stageNum = 1;
	}
	else if(m_stageNum < 1)
	{
		m_stageNum = m_maxStageNum;
	}
	GetManager().lock()->GetGameObject("RemainUI").lock()->GetGameComponent<SpriteAnimationComponent>()->SetHorizontalAnim(m_stageNum - 1);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::StageSelectManagerComponent::Clone()
{
	return ObjectFactory::Create<StageSelectManagerComponent>();
}
