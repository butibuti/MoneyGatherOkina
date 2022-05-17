#include "stdafx_u.h"
#include "WaveManager.h"
#include "StartPopUpComponent.h"
#include "EnemySpawnManager.h"
#include "Player.h"
#include "InputManager.h"
#include "EnemySpawner.h"
#include "SceneChangeAnimationComponent.h"
#include "GameOverManagerComponent.h"

void ButiEngine::WaveManager::OnUpdate()
{
	if (!m_isSceneStart)
	{
		m_isSceneStart = true;
		m_vwp_sceneChangeAnimationComponent.lock()->SceneStart();
	}

	MoveWave();

	if (m_isWaveTime)
	{
		//ウェーブ中

		if (m_vwp_playerComponent.lock()->IsDead())
		{
			m_isGameOver = true;
		}

	}
	else
	{
		//ウェーブ外

		//最終ウェーブをクリアしていたら
		if (m_waveNum >= m_maxWaveNum)
		{
			m_isLastWaveClear = true;
		}

		if (m_isPopupSpawn && !m_isLastWaveClear)
		{
			m_isPopupSpawn = false;
			//ポップをスポーンさせる
			m_vwp_startPopUpObject.lock()->AppearPopUp();
		}
	}

	StageClearAnimation();
	GameOverAnimation();

}

void ButiEngine::WaveManager::OnSet()
{
}

void ButiEngine::WaveManager::Start()
{
	m_vwp_startPopUpObject = GetManager().lock()->GetGameObject("StartPopUpObject").lock()->GetGameComponent<StartPopUpComponent>();
	m_vwp_playerComponent = GetManager().lock()->GetGameObject("Player").lock()->GetGameComponent<Player>();
	auto sceneChangeAnimation = GetManager().lock()->AddObjectFromCereal("SceneChangeAnimation");
	m_vwp_sceneChangeAnimationComponent = sceneChangeAnimation.lock()->GetGameComponent<SceneChangeAnimationComponent>();
	m_waveNum = 0;
	m_maxWaveNum = 1;
	m_clearAnimationTime = 0;
	m_isWaveTime = false;
	m_isPopupSpawn = false;
	m_isLastWaveClear = false;
	m_isGameOver = false;
	m_isNextSceneButton = false;
	m_isGameOverButton = false;
	m_isNextScene = false;
	m_isSceneStart = false;

	m_enemyDeadCount = 0;
	m_maxEnemyCount = 10;
	m_enemySpawnCount = 0;
}

void ButiEngine::WaveManager::OnShowUI()
{
	GUI::BulletText("WaveNum");
	GUI::InputInt("##waveNum", m_waveNum);
	GUI::BulletText("MaxWaveNum");
	GUI::InputInt("##maxWaveNum", m_maxWaveNum);
	GUI::BulletText("MaxEnemyCount");
	GUI::InputInt("##maxEnemyCount", m_maxEnemyCount);
}

void ButiEngine::WaveManager::WaveStart()
{
	m_isWaveTime = true;
}

void ButiEngine::WaveManager::AddEnemyDeadCount()
{
	m_enemyDeadCount++;
}

void ButiEngine::WaveManager::AddSpawnCount()
{
	m_enemySpawnCount++;
}

void ButiEngine::WaveManager::FixWaveNum()
{
	//Wave番号を範囲内に修正
	if (m_waveNum < 0)
	{
		m_waveNum = 0;
	}
	else if (m_waveNum > m_maxWaveNum)
	{
		m_waveNum = m_maxWaveNum;
	}
}

void ButiEngine::WaveManager::MoveWave()
{
	//最終ウェーブをクリアしていたら通らないようにする
	if (m_isLastWaveClear) return;

	//プレイヤーがポップに触れたらウェーブ開始
	if (m_vwp_startPopUpObject.lock()->IsHitPlayerFlag() && !m_isWaveTime)
	{
		//ウェーブ番号を進める
		m_waveNum++;
		FixWaveNum();

		//ウェーブ開始
		m_isWaveTime = true;

		//ポップを引っ込める
		m_vwp_startPopUpObject.lock()->DisappearPopUp();

		//敵をスポーンさせる
		SpawnEnemy();
	}

	
	////仮でボタンを5回押したらウェーブをクリアできるようにしている
	//if (InputManager::IsTriggerCancelKey() && m_isWaveTime)
	//{
	//	m_enemyCount++;
	//}
	//フィールド内の敵をすべて倒していたら
	if (m_enemyDeadCount >= m_maxEnemyCount && m_isWaveTime)
	{
		WaveFinish();
	}
}

void ButiEngine::WaveManager::SpawnEnemy()
{
	////ウェーブ番号に応じて出現させる敵のパターンや配置を変える
	////何ステージ目・何ウェーブ目・敵の名前・位置

	//auto sceneName = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneInformation()->GetSceneName();
	//std::int16_t underScoreIndex = sceneName.find("_");
	//auto size = sceneName.size();
	//std::string stageNum = sceneName.substr(underScoreIndex + 1, size);

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
	auto enemy0 = GetManager().lock()->AddObjectFromCereal("EnemySpawner").lock()->GetGameComponent<EnemySpawner>();
	enemy0->SetType(0);
	auto enemy1 = GetManager().lock()->AddObjectFromCereal("EnemySpawner").lock()->GetGameComponent<EnemySpawner>();
	enemy1->SetType(1);
	auto enemy2 = GetManager().lock()->AddObjectFromCereal("EnemySpawner").lock()->GetGameComponent<EnemySpawner>();
	enemy2->SetType(2);
	auto enemy3 = GetManager().lock()->AddObjectFromCereal("EnemySpawner").lock()->GetGameComponent<EnemySpawner>();
	enemy3->SetType(3);

}

void ButiEngine::WaveManager::WaveFinish()
{
	//ウェーブ終了
	m_isWaveTime = false;
	m_isPopupSpawn = true;
	m_enemyDeadCount = 0;
}

void ButiEngine::WaveManager::StageClearAnimation()
{
	//最終ウェーブクリア後に通るようにする
	if (!m_isLastWaveClear) return;

	//仮
	if (m_clearAnimationTime < 120)
	{
		m_clearAnimationTime++;
	}
	else
	{
		m_isNextSceneButton = true;
	}

	//ステージセレクトへ
	if (InputManager::IsTriggerDecideKey() && m_isNextSceneButton)
	{
		auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
		std::string sceneName = "StageSelect";
		sceneManager->RemoveScene(sceneName);
		sceneManager->LoadScene(sceneName);
		sceneManager->ChangeScene(sceneName);
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

	if (InputManager::IsTriggerDecideKey() && m_vwp_gameOverManagerComponent.lock()->IsNext())
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
			std::string sceneName = "GamePlay";
			sceneManager->RemoveScene(sceneName);
			sceneManager->LoadScene(sceneName);
			sceneManager->ChangeScene(sceneName);
		}
		else
		{
			auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
			std::string sceneName = "Title";
			sceneManager->RemoveScene(sceneName);
			sceneManager->LoadScene(sceneName);
			sceneManager->ChangeScene(sceneName);
		}

		////ウェーブの途中からやり直す
		//m_waveNum--;
		//m_isGameOver = false;
		//m_vwp_playerComponent.lock()->Revival(); //プレイヤー蘇生
		//WaveFinish();
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::WaveManager::Clone()
{
	return ObjectFactory::Create<WaveManager>();
}
