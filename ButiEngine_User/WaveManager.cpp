#include "stdafx_u.h"
#include "WaveManager.h"
#include "EnemySpawnManager.h"
#include "Player.h"
#include "InputManager.h"
#include "EnemySpawner.h"
#include "SceneChangeAnimationComponent.h"
#include "GameOverManagerComponent.h"
#include "StageClearManagerComponent.h"
#include "AllStageClearManagerComponent.h"
#include "StageProgressUIComponent.h"
#include "PauseManagerComponent.h"
#include "WorldSpeedManager.h"
#include "CameraComponent.h"
#include "SoundPlayerComponent.h"
#include "WorkerSpawner.h"
#include "StageSelectManagerComponent.h"
#include "TutorialUI.h"

std::int32_t ButiEngine::WaveManager::m_tutorialClearPoint = 300;
std::int32_t ButiEngine::WaveManager::m_stageClearPoint = 1000;
std::int32_t ButiEngine::WaveManager::m_retryPoint = 0;

void ButiEngine::WaveManager::OnUpdate()
{
	if (!m_isSceneStart)
	{
		m_isSceneStart = true;
		m_vwp_sceneChangeAnimationComponent.lock()->SceneStart();
	}

	if (!m_vwp_sceneChangeAnimationComponent.lock()->IsAnimation() && !m_isSpawnAnimation)
	{
		m_isSpawnAnimation = true;
		m_vlp_spawnTimer->Start();
	}

	if (m_isSpawnAnimation)
	{
		SpawnAnimation();
	}

	if (!m_isGameStart)
	{
		return;
	}

	if (m_vwp_playerComponent.lock()->IsDead() && !m_isClear)
	{
		if (!m_isAdvanceGameOver)
		{
			m_vwp_worldSpeedManagerComponent.lock()->SetSpeed(0.3f, 120);
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->StopBGM();
		}
		m_isAdvanceGameOver = true;
		if (m_vlp_advanceGameOverTimer->Update())
		{
			if (!m_isAdvanceNextGameOver)
			{
				auto cameraComponent = GetManager().lock()->GetGameObject("Camera").lock()->GetGameComponent<CameraComponent>();
				cameraComponent->SetZoomOperationNum(0);
				m_isAdvanceNextGameOver = true;

				m_vlp_advanceGameOverTimer->Reset();
				m_vlp_advanceGameOverTimer->ChangeCountFrame(30);
			}
			else
			{
				if (!m_isGameOver)
				{
					m_vwp_playerComponent.lock()->Dead();
				}
				m_isGameOver = true;
			}
		}
		
	}
	//クリアしているか
	if (m_point >= m_clearPoint && !m_isAdvanceGameOver)
	{
		m_isClear = true;
	}

#ifdef DEBUG
	if (GameDevice::GetInput()->TriggerKey(Keys::L))
	{
		m_isClear = true;
	}
#endif // DEBUG

	StageClearAnimation();
	GameOverAnimation();
	PauseAnimation();
	StageProgressAnimation();
}

void ButiEngine::WaveManager::OnSet()
{
	auto oneFrameObject = GetManager().lock()->GetGameObject("DrawObject_OneFrame");
	if (!oneFrameObject.lock())
	{
		oneFrameObject = GetManager().lock()->AddObjectFromCereal("DrawObject_OneFrame");
	}
	m_vlp_advanceGameOverTimer = ObjectFactory::Create<AbsoluteTimer>(120);

	m_sceneName = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneInformation()->GetSceneName();

	std::uint16_t spawnFrame = 150;
	std::uint16_t spawnIntervalFrame = 70;

	if (m_sceneName == "Stage_0")
	{
		m_isTutorial = true;
		spawnFrame = 50;
		spawnIntervalFrame = 0;
		m_clearPoint = m_tutorialClearPoint;
	}
	else if (m_sceneName == "Stage_1")
	{
		m_isTutorial = false;
		m_clearPoint = m_stageClearPoint;
	}

	m_vlp_spawnTimer = ObjectFactory::Create<AbsoluteTimer>(spawnFrame);
	m_vlp_spawnIntervalTimer = ObjectFactory::Create<AbsoluteTimer>(spawnIntervalFrame);
}

void ButiEngine::WaveManager::Start()
{
	m_vlp_advanceGameOverTimer->Start();
	m_vwp_playerComponent = GetManager().lock()->GetGameObject("Player").lock()->GetGameComponent<Player>();
	auto sceneChangeAnimation = GetManager().lock()->AddObjectFromCereal("SceneChangeAnimation");
	m_vwp_sceneChangeAnimationComponent = sceneChangeAnimation.lock()->GetGameComponent<SceneChangeAnimationComponent>();
	m_vwp_stageProgressUIComponent = GetManager().lock()->AddObjectFromCereal("StageProgressUI_Inline").lock()->GetGameComponent<StageProgressUIComponent>();
	m_vwp_pauseManagerComponent = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManagerComponent>();
	m_vwp_worldSpeedManagerComponent = GetManager().lock()->GetGameObject("WorldSpeedManager").lock()->GetGameComponent<WorldSpeedManager>();
	m_vwp_soundPlayerComponent = GetManager().lock()->GetGameObject("SoundPlayer").lock()->GetGameComponent<SoundPlayerComponent>();

	m_isGameStart = false;
	m_isClear = false;
	m_isGameOver = false;
	m_isAdvanceGameOver = false;
	m_isAdvanceNextGameOver = false;
	m_isNextSceneButton = false;
	m_isGameOverButton = false;
	m_isNextScene = false;
	m_isSceneStart = false;
	m_isSpawnAnimation = false;
	m_isSpawnedPlayer = false;
	m_isSpawnWorker = false;

	m_point = m_retryPoint;
	m_enemySpawnCount = 0;

	m_vwp_soundPlayerComponent.lock()->PlayBGM(SoundTag("Sound/BGM1.wav"));
}

void ButiEngine::WaveManager::OnShowUI()
{
	GUI::BulletText("TutorialClearPoint");
	GUI::InputInt("##TutorialClearPoint", m_tutorialClearPoint);
	GUI::BulletText("StageClearPoint");
	GUI::InputInt("##StageClearPoint", m_stageClearPoint);
	GUI::BulletText("Point:%d", m_point);
}

void ButiEngine::WaveManager::AddPoint(const std::int32_t arg_progressPoint)
{
	if (m_isClear || m_isAdvanceGameOver) { return; }
	m_point += arg_progressPoint;
}

void ButiEngine::WaveManager::AddSpawnCount()
{
	m_enemySpawnCount++;
}

void ButiEngine::WaveManager::SpawnEnemySpawner()
{
	////ウェーブ番号に応じて出現させる敵のパターンや配置を変える
	////何ステージ目・何ウェーブ目・敵の名前・位置

	auto sceneName = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneInformation()->GetSceneName();
	std::int16_t underScoreIndex = sceneName.find("_");
	auto size = sceneName.size();
	std::string stageNum = sceneName.substr(underScoreIndex + 1, size);

	//std::string fileName = "EnemyData/0_" /*+ stageNum + "_"*/ + std::to_string(m_waveNum) + ".enemyData";

	//std::vector<EnemySpawnData> vec_enemySpawnDatas;
	//InputCereal(vec_enemySpawnDatas, fileName);

	//for (auto enemyData : vec_enemySpawnDatas)
	//{
	//	auto enemy = GetManager().lock()->AddObjectFromCereal(enemyData.m_enemyName);
	//	auto transformData = enemyData.m_vlp_enemyTransform;
	//	enemy.lock()->transform->SetLocalPosition(transformData->GetLocalPosition());
	//	enemy.lock()->transform->SetLocalRotation(transformData->GetLocalRotation());

	//	m_enemyCount++;
	//}


	//スポナーを生成
	for (std::int8_t i = 0; i < 4; i++)
	{
		auto flySpawner = GetManager().lock()->AddObjectFromCereal("EnemySpawner").lock()->GetGameComponent<EnemySpawner>();
		flySpawner->SetType(0);
		flySpawner->SetStageNum(stageNum);
		flySpawner->SetSpawnerIndex(i);
		auto stalkerSpawner = GetManager().lock()->AddObjectFromCereal("EnemySpawner").lock()->GetGameComponent<EnemySpawner>();
		stalkerSpawner->SetType(1);
		stalkerSpawner->SetStageNum(stageNum);
		stalkerSpawner->SetSpawnerIndex(i);
		auto kibaSpawner = GetManager().lock()->AddObjectFromCereal("EnemySpawner").lock()->GetGameComponent<EnemySpawner>();
		kibaSpawner->SetType(2);
		kibaSpawner->SetStageNum(stageNum);
		kibaSpawner->SetSpawnerIndex(i);
		auto volcanoSpawner = GetManager().lock()->AddObjectFromCereal("EnemySpawner").lock()->GetGameComponent<EnemySpawner>();
		volcanoSpawner->SetType(3);
		volcanoSpawner->SetStageNum(stageNum);
		volcanoSpawner->SetSpawnerIndex(i);
	}

}

void ButiEngine::WaveManager::StageClearAnimation()
{
	//クリア時に通るようにする
	if (!m_isClear) return;

	if (m_sceneName == "Stage_1")
	{
		if (!m_vwp_allStageClearManagerComponent.lock())
		{
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->StopBGM();
			auto stageClearManager = GetManager().lock()->AddObjectFromCereal("AllStageClearManager");
			m_vwp_allStageClearManagerComponent = stageClearManager.lock()->GetGameComponent<AllStageClearManagerComponent>();
		}

		//ステージセレクトへ
		if (InputManager::IsTriggerDecideKey() && m_vwp_allStageClearManagerComponent.lock()->IsNext() && !m_isNextScene)
		{
			m_vwp_soundPlayerComponent.lock()->PlayIsolateSE(SoundTag("Sound/UI_Enter.wav"));
			m_vwp_sceneChangeAnimationComponent.lock()->SceneEnd();
			m_isNextScene = true;
		}
		else if (!m_vwp_sceneChangeAnimationComponent.lock()->IsAnimation() && m_isNextScene)
		{
			auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
			std::string sceneName = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneInformation()->GetSceneName();
			sceneManager->RemoveScene(sceneName);
			sceneName = "StageSelect";
			sceneManager->ChangeScene(sceneName);

			m_retryPoint = 0;

			m_vwp_worldSpeedManagerComponent.lock()->SetSpeed(1.0f);
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->SetMasterVolume(1.0f);
		}
	}
	else
	{
		if (!m_vwp_stageClearManagerComponent.lock())
		{
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->StopBGM();
			auto stageClearManager = GetManager().lock()->AddObjectFromCereal("StageClearManager");
			m_vwp_stageClearManagerComponent = stageClearManager.lock()->GetGameComponent<StageClearManagerComponent>();
		}

		//ステージセレクトへ
		if (InputManager::IsTriggerDecideKey() && m_vwp_stageClearManagerComponent.lock()->IsNext() && !m_isNextScene)
		{
			m_vwp_soundPlayerComponent.lock()->PlayIsolateSE(SoundTag("Sound/UI_Enter.wav"));
			m_vwp_sceneChangeAnimationComponent.lock()->SceneEnd();
			m_isNextScene = true;
		}
		else if (!m_vwp_sceneChangeAnimationComponent.lock()->IsAnimation() && m_isNextScene)
		{
			auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
			std::string sceneName = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneInformation()->GetSceneName();
			sceneManager->RemoveScene(sceneName);
			sceneName = "StageSelect";
			sceneManager->ChangeScene(sceneName);

			m_retryPoint = 0;

			StageSelectManagerComponent::AddStageNum();

			m_vwp_worldSpeedManagerComponent.lock()->SetSpeed(1.0f);
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->SetMasterVolume(1.0f);
		}
	}

	
}

void ButiEngine::WaveManager::GameOverAnimation()
{
	//ゲームオーバーの時に通るようにする
	if (!m_isGameOver) return;

	if (!m_vwp_gameOverManagerComponent.lock())
	{
		auto gameOverManager = GetManager().lock()->AddObjectFromCereal("GameOverManager");
		m_vwp_gameOverManagerComponent = gameOverManager.lock()->GetGameComponent<GameOverManagerComponent>();
	}

	if (!m_isNextScene && m_vwp_gameOverManagerComponent.lock()->IsNext())
	{
		m_vwp_sceneChangeAnimationComponent.lock()->SceneEnd();
		m_isNextScene = true;
	}
	else if (!m_vwp_sceneChangeAnimationComponent.lock()->IsAnimation() && m_isNextScene)
	{
		//リトライかタイトルか
		if (m_vwp_gameOverManagerComponent.lock()->IsRetry())
		{
			auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
			std::string sceneName = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneInformation()->GetSceneName();
			sceneManager->RemoveScene(sceneName);
			sceneManager->LoadScene(sceneName);
			sceneManager->ChangeScene(sceneName);

			m_vwp_worldSpeedManagerComponent.lock()->SetSpeed(1.0f);
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->SetMasterVolume(1.0f);
		}
		else
		{
			auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
			std::string sceneName = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneInformation()->GetSceneName();
			sceneManager->RemoveScene(sceneName);
			sceneName = "StageSelect";
			sceneManager->LoadScene(sceneName);
			sceneManager->ChangeScene(sceneName);

			m_retryPoint = 0;

			m_vwp_worldSpeedManagerComponent.lock()->SetSpeed(1.0f);
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->SetMasterVolume(1.0f);
		}

		////ウェーブの途中からやり直す
		//m_waveNum--;
		//m_isGameOver = false;
		//m_vwp_playerComponent.lock()->Revival(); //プレイヤー蘇生
		//WaveFinish();
	}
}

void ButiEngine::WaveManager::PauseAnimation()
{
	//ポーズ中のみ通るようにする
	if (!m_vwp_pauseManagerComponent.lock()->IsPause()) { return; }

	if (InputManager::IsTriggerDecideKey() && m_vwp_pauseManagerComponent.lock()->IsNext())
	{
		if (!m_vwp_pauseManagerComponent.lock()->IsBack())
		{
			m_vwp_sceneChangeAnimationComponent.lock()->SceneEnd();
			m_isNextScene = true;
		}
	}
	else if (!m_vwp_sceneChangeAnimationComponent.lock()->IsAnimation() && m_isNextScene)
	{
		auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
		std::string sceneName = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneInformation()->GetSceneName();
		sceneManager->RemoveScene(sceneName);
		sceneName = "StageSelect";
		sceneManager->LoadScene(sceneName);
		sceneManager->ChangeScene(sceneName);

		m_retryPoint = 0;

		m_vwp_worldSpeedManagerComponent.lock()->SetSpeed(1.0f);
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->SetMasterVolume(1.0f);
	}
}

void ButiEngine::WaveManager::StageProgressAnimation()
{
	if (!m_vwp_stageProgressUIComponent.lock()) { return; }

	auto rate = (float)m_point / (float)m_clearPoint;
	m_vwp_stageProgressUIComponent.lock()->SetRate(rate);
}

void ButiEngine::WaveManager::SpawnAnimation()
{
	if (m_isGameStart) { return; }

	if (!m_isSpawnedPlayer)
	{
		m_vwp_playerComponent.lock()->Spawn();
		m_vlp_spawnIntervalTimer->Start();
		m_isSpawnedPlayer = true;
	}
	else if(m_vlp_spawnIntervalTimer->Update())
	{
		m_vlp_spawnIntervalTimer->Stop();
		m_isSpawnWorker = true;
	}

	if (m_isSpawnWorker)
	{
		GetManager().lock()->GetGameObject("WorkerSpawner").lock()->GetGameComponent<WorkerSpawner>()->SpawnStart();
		m_isSpawnWorker = false;
	}

	if (m_vlp_spawnTimer->Update())
	{
		m_vlp_spawnTimer->Stop();

		m_isGameStart = true;
		GetManager().lock()->GetGameObject("Camera").lock()->GetGameComponent<CameraComponent>()->SetZoomOperationNum(2);

		if (!m_isTutorial)
		{
			//エネミースポナーをスポーンさせる
			SpawnEnemySpawner();
		}
		else
		{
			GetManager().lock()->GetGameObject("TutorialUI_Move").lock()->GetGameComponent<TutorialUI>()->Appear();
		}
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::WaveManager::Clone()
{
	return ObjectFactory::Create<WaveManager>();
}
