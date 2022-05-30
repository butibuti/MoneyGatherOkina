#include "stdafx_u.h"
#include "StageSelectManagerComponent.h"
#include "SceneChangeAnimationComponent.h"
#include "InputManager.h"
#include "FloatMotionComponent.h"
#include "SoundPlayerComponent.h"
#include "EnemyScaleAnimationComponent.h"
#include "ButiEngineHeader/Header/GameObjects/DefaultGameComponent/SpriteAnimationComponent.h"

std::int16_t ButiEngine::StageSelectManagerComponent::m_stageNum = 0;

void ButiEngine::StageSelectManagerComponent::OnUpdate()
{
	//if (!m_waitTimer->Update_continue()) { return; }

	if (!m_isSceneChange&&InputManager::IsTriggerRightKey()&&!m_vwp_gamePlayChangeAnimation.lock()->IsAnimation())
	{
		m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/UI_Select.wav"));
		m_stageNum++;
		m_animationRate = 0;
	}
	else if(!m_isSceneChange&&InputManager::IsTriggerLeftKey() && !m_vwp_gamePlayChangeAnimation.lock()->IsAnimation())
	{
		m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/UI_Select.wav"));
		m_stageNum--;
		m_animationRate = 0;
	}

	FixStageNum();

	if (m_animationRate < 1.0f)
	{
		m_animationRate += 0.1f;
	}

	if (m_stageNum == 0)
	{
		if (m_scaleRate > 0.0f)
		{
			m_scaleRate -= 0.2f;
		}
		else
		{
			m_scaleRate = 0;
		}
		m_kibaPosition.x = MathHelper::Lerp(m_kibaPosition.x, m_rightPosition.x, Easing::EaseOutExpo(m_animationRate));
		m_crystalPosition.x = MathHelper::Lerp(m_crystalPosition.x, m_centerPosition.x, Easing::EaseOutExpo(m_animationRate));
	}
	else
	{
		if (m_scaleRate < 1.0f)
		{
			m_scaleRate += 0.2f;
		}
		else
		{
			m_scaleRate = 1.0f;
		}
		m_kibaPosition.x = MathHelper::Lerp(m_kibaPosition.x, m_centerPosition.x, Easing::EaseOutExpo(m_animationRate));
		m_crystalPosition.x = MathHelper::Lerp(m_crystalPosition.x, m_leftPosition.x, Easing::EaseOutExpo(m_animationRate));
	}

	float crystalScale = 1.0f - m_scaleRate;
	m_vwp_crystalScaleAnimationComponent.lock()->SetScaleRate(crystalScale);
	m_vwp_enemyScaleAnimationComponent.lock()->SetScaleRate(m_scaleRate);

	m_vwp_kiba.lock()->transform->SetLocalPosition(m_kibaPosition);
	m_vwp_crystal.lock()->transform->SetLocalPosition(m_crystalPosition);

	if (InputManager::IsTriggerDecideKey() && !m_vwp_gamePlayChangeAnimation.lock()->IsAnimation()&&!m_isSceneChange)
	{
		m_vwp_gamePlayChangeAnimation.lock()->SceneEnd();
		m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/UI_Enter.wav"));
		m_isSceneChange = true;
	}

	if (m_isSceneChange && !m_vwp_gamePlayChangeAnimation.lock()->IsAnimation()) 
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
	m_maxStageNum = 1;
	m_scaleRate = 0;
	m_animationRate = 0;
	m_isSceneChange = false;

	m_centerPosition = Vector3(0.0f, 0.0f, 0.0f);
	m_rightPosition = Vector3(6.0f, 0.0f, 0.0f);
	m_leftPosition = Vector3(-6.0f, 0.0f, 0.0f);

	//auto sceneChangeAnimation = GetManager().lock()->GetGameObject("SceneChangeAnimation");
	m_vwp_gamePlayChangeAnimation = gameObject.lock()->GetGameComponent<SceneChangeAnimationComponent>();

	m_vwp_gamePlayChangeAnimation.lock()->SceneStart();
	m_vwp_soundPlayerComponent = GetManager().lock()->GetGameObject("SoundPlayer").lock()->GetGameComponent<SoundPlayerComponent>();
	
	m_vwp_kiba = GetManager().lock()->GetGameObject("SelectKiba");
	m_vwp_enemyScaleAnimationComponent = m_vwp_kiba.lock()->GetGameComponent<EnemyScaleAnimationComponent>();
	m_vwp_enemyScaleAnimationComponent.lock()->SetMaxPlusScale(1.0f);
	m_vwp_enemyScaleAnimationComponent.lock()->SetScaleRate(1.0f);

	m_vwp_crystal = GetManager().lock()->GetGameObject("SelectCrystal");
	m_vwp_crystalScaleAnimationComponent = m_vwp_crystal.lock()->GetGameComponent<EnemyScaleAnimationComponent>();
	m_vwp_crystalScaleAnimationComponent.lock()->SetMaxPlusScale(1.0f);
	m_vwp_crystalScaleAnimationComponent.lock()->SetScaleRate(1.0f);

	if (m_stageNum == 0)
	{
		m_kibaPosition = m_rightPosition;
		m_crystalPosition = m_centerPosition;
	}
	else
	{
		m_kibaPosition = m_centerPosition;
		m_crystalPosition = m_leftPosition;
	}
	m_vwp_kiba.lock()->transform->SetLocalPosition(m_kibaPosition);
	m_vwp_crystal.lock()->transform->SetLocalPosition(m_crystalPosition);

	m_vwp_soundPlayerComponent.lock()->PlayBGM(SoundTag("Sound/BGM2.wav"));

	GetCamera("BloomSource")->vlp_transform->SetBaseTransform(GetCamera("main")->vlp_transform);
	
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
	m_stageNum++;
	m_stageNum = min(m_stageNum, m_maxStageNum);
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
