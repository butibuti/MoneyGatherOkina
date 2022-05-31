#include "stdafx_u.h"
#include "StageSelectManagerComponent.h"
#include "SceneChangeAnimationComponent.h"
#include "InputManager.h"
#include "FloatMotionComponent.h"
#include "SoundPlayerComponent.h"
#include "EnemyScaleAnimationComponent.h"
#include "MoveAnimationComponent.h"
#include "ButiEngineHeader/Header/GameObjects/DefaultGameComponent/SpriteAnimationComponent.h"

std::int16_t ButiEngine::StageSelectManagerComponent::m_stageNum = 0;

void ButiEngine::StageSelectManagerComponent::OnUpdate()
{
	if (!m_isSceneChange && InputManager::IsTriggerRightKey() && !m_vwp_gamePlayChangeAnimation.lock()->IsAnimation())
	{
		m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/UI_Select.wav"));
		m_stageNum++;
		m_vwp_moveAnimationComponent.lock()->SetIsActive(true);
	}
	else if (!m_isSceneChange && InputManager::IsTriggerLeftKey() && !m_vwp_gamePlayChangeAnimation.lock()->IsAnimation())
	{
		m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/UI_Select.wav"));
		m_stageNum--;
		m_vwp_moveAnimationComponent.lock()->SetIsActive(true);
	}
	FixStageNum();
	ScaleRateUpdate();

	if (m_stageNum == 0)
	{
		m_vwp_moveAnimationComponent.lock()->SetEndPosition(m_centerPosition);
		m_vwp_modeSpriteAnimComponent.lock()->SetHorizontalAnim(1);
		m_vwp_explanSpriteAnimComponent.lock()->SetHorizontalAnim(1);
	}
	else if (m_stageNum == 1)
	{
		m_vwp_moveAnimationComponent.lock()->SetEndPosition(m_leftPosition);
		m_vwp_modeSpriteAnimComponent.lock()->SetHorizontalAnim(0);
		m_vwp_explanSpriteAnimComponent.lock()->SetHorizontalAnim(0);
	}
	else
	{
		m_vwp_moveAnimationComponent.lock()->SetEndPosition(m_rightPosition);
		m_vwp_modeSpriteAnimComponent.lock()->SetHorizontalAnim(2);
		m_vwp_explanSpriteAnimComponent.lock()->SetHorizontalAnim(2);
	}

	m_vwp_enemyScaleAnimationComponent.lock()->SetMaxPlusScale(m_kibaScaleRate);
	m_vwp_crystalScaleAnimationComponent.lock()->SetMaxPlusScale(m_crystalScaleRate);
	m_vwp_endGameScaleAnimationComponent.lock()->SetMaxPlusScale(m_endGameScaleRate);

	if (!m_vlp_waitTimer->Update_continue()) { return; }
	if (InputManager::IsTriggerDecideKey() && !m_vwp_gamePlayChangeAnimation.lock()->IsAnimation()&&!m_isSceneChange)
	{
		m_vwp_gamePlayChangeAnimation.lock()->SceneEnd();
		m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/UI_Enter.wav"));
		m_isSceneChange = true;
	}

	if (m_isSceneChange && !m_vwp_gamePlayChangeAnimation.lock()->IsAnimation()) 
	{
		//次のシーンへ
		if (m_vlp_nextTimer->Update())
		{
			NextScene();
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->StopBGM();
		}
	}
}

void ButiEngine::StageSelectManagerComponent::OnSet()
{
	m_vlp_waitTimer = ObjectFactory::Create<AbsoluteTimer>(60);
	m_vlp_nextTimer = ObjectFactory::Create<AbsoluteTimer>(4);
}

void ButiEngine::StageSelectManagerComponent::OnShowUI()
{
}

void ButiEngine::StageSelectManagerComponent::Start()
{
	m_vlp_waitTimer->Start();
	m_vlp_nextTimer->Start();
	m_maxStageNum = 2;
	m_kibaScaleRate = 0;
	m_crystalScaleRate = 0;
	m_endGameScaleRate = 0;
	m_animationRate = 0;
	m_isSceneChange = false;

	m_centerPosition = Vector3(0.0f, 0.0f, 0.0f);
	m_rightPosition = Vector3(8.0f, 0.0f, 0.0f);
	m_leftPosition = Vector3(-8.0f, 0.0f, 0.0f);

	//auto sceneChangeAnimation = GetManager().lock()->GetGameObject("SceneChangeAnimation");
	m_vwp_gamePlayChangeAnimation = gameObject.lock()->GetGameComponent<SceneChangeAnimationComponent>();

	m_vwp_gamePlayChangeAnimation.lock()->SceneStart();
	m_vwp_soundPlayerComponent = GetManager().lock()->GetGameObject("SoundPlayer").lock()->GetGameComponent<SoundPlayerComponent>();
	
	auto titleLogo = GetManager().lock()->GetGameObject("TitleLogo").lock()->GetGameComponent<FloatMotionComponent>();
	titleLogo->SetAmplitude(15.0f);
	titleLogo->SetMotionSpeed(0.03f);
	titleLogo->SetIsRandomSpeed(false);
	titleLogo->SetIsAbsolute(true);

	m_vwp_moveParent = GetManager().lock()->GetGameObject("MoveParent");
	m_vwp_moveAnimationComponent = m_vwp_moveParent.lock()->GetGameComponent<MoveAnimationComponent>();
	m_vwp_moveAnimationComponent.lock()->SetEndPosition(m_centerPosition);
	m_vwp_moveAnimationComponent.lock()->SetSpeed(0.25f);
	m_vwp_moveAnimationComponent.lock()->SetIsActive(true);

	m_vwp_kiba = GetManager().lock()->GetGameObject("SelectKiba");
	m_vwp_kiba.lock()->transform->SetBaseTransform(m_vwp_moveParent.lock()->transform);
	m_vwp_enemyScaleAnimationComponent = m_vwp_kiba.lock()->GetGameComponent<EnemyScaleAnimationComponent>();
	m_vwp_enemyScaleAnimationComponent.lock()->SetMaxPlusScale(1.0f);
	m_vwp_enemyScaleAnimationComponent.lock()->SetScaleRate(1.0f);

	m_vwp_crystal = GetManager().lock()->GetGameObject("SelectCrystal");
	m_vwp_crystal.lock()->transform->SetBaseTransform(m_vwp_moveParent.lock()->transform);
	m_vwp_crystalScaleAnimationComponent = m_vwp_crystal.lock()->GetGameComponent<EnemyScaleAnimationComponent>();
	m_vwp_crystalScaleAnimationComponent.lock()->SetMaxPlusScale(1.0f);
	m_vwp_crystalScaleAnimationComponent.lock()->SetScaleRate(1.0f);

	m_vwp_endGame = GetManager().lock()->GetGameObject("SelectEndGame");
	m_vwp_endGame.lock()->transform->SetBaseTransform(m_vwp_moveParent.lock()->transform);
	m_vwp_endGameScaleAnimationComponent = m_vwp_endGame.lock()->GetGameComponent<EnemyScaleAnimationComponent>();
	m_vwp_endGameScaleAnimationComponent.lock()->SetMaxPlusScale(1.0f);
	m_vwp_endGameScaleAnimationComponent.lock()->SetScaleRate(1.0f);

	m_vwp_modeSpriteAnimComponent = GetManager().lock()->GetGameObject("GameModeText").lock()->GetGameComponent<SpriteAnimationComponent>();
	m_vwp_explanSpriteAnimComponent = GetManager().lock()->GetGameObject("GameExplanationText").lock()->GetGameComponent<SpriteAnimationComponent>();

	m_vwp_soundPlayerComponent.lock()->PlayBGM(SoundTag("Sound/BGM2.wav"));

	//竹渕が書いたブルーム用カメラにmainを親としてセットする処理
	GetCamera("BloomSource")->vlp_transform->SetBaseTransform(GetCamera("main")->vlp_transform,true);
	GetCamera("IsolateEffect")->vlp_transform->SetBaseTransform(GetCamera("main")->vlp_transform,true);
	GetCamera("IsolateEffectBloom")->vlp_transform->SetBaseTransform(GetCamera("main")->vlp_transform,true);
	
}

void ButiEngine::StageSelectManagerComponent::End()
{
}

void ButiEngine::StageSelectManagerComponent::NextScene()
{
	if (m_stageNum == 2)
	{
		GameDevice::SetIsEnd(true);
		return;
	}

	auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
	//std::string sceneName = "GamePlay";
	std::string sceneName = "Stage_" + std::to_string(m_stageNum);
	GUI::Console("Stage_" + std::to_string(m_stageNum)+"への移動");
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

void ButiEngine::StageSelectManagerComponent::AddStageNum()
{
	m_stageNum++;
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

void ButiEngine::StageSelectManagerComponent::ScaleRateUpdate()
{
	if (m_stageNum == 0)
	{
		if (m_kibaScaleRate > 0)
		{
			m_kibaScaleRate -= 0.2f;
		}
		else
		{
			m_kibaScaleRate = 0;
		}
		if (m_endGameScaleRate > 0)
		{
			m_endGameScaleRate -= 0.2f;
		}
		else
		{
			m_endGameScaleRate = 0;
		}
		if (m_crystalScaleRate < 2)
		{
			m_crystalScaleRate += 0.2f;
		}
		else
		{
			m_crystalScaleRate = 2;
		}
	}
	else if (m_stageNum == 1)
	{
		if (m_crystalScaleRate > 0)
		{
			m_crystalScaleRate -= 0.2f;
		}
		else
		{
			m_crystalScaleRate = 0;
		}
		if (m_endGameScaleRate > 0)
		{
			m_endGameScaleRate -= 0.2f;
		}
		else
		{
			m_endGameScaleRate = 0;
		}
		if (m_kibaScaleRate < 2)
		{
			m_kibaScaleRate += 0.2f;
		}
		else
		{
			m_kibaScaleRate = 2;
		}
	}
	else
	{
		if (m_crystalScaleRate > 0)
		{
			m_crystalScaleRate -= 0.2f;
		}
		else
		{
			m_crystalScaleRate = 0;
		}
		if (m_kibaScaleRate > 0)
		{
			m_kibaScaleRate -= 0.2f;
		}
		else
		{
			m_kibaScaleRate = 0;
		}
		if (m_endGameScaleRate < 2)
		{
			m_endGameScaleRate += 0.2f;
		}
		else
		{
			m_endGameScaleRate = 2;
		}
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::StageSelectManagerComponent::Clone()
{
	return ObjectFactory::Create<StageSelectManagerComponent>();
}
