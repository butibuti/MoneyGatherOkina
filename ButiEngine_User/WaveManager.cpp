#include "stdafx_u.h"
#include "WaveManager.h"
#include "StartPopUpComponent.h"
#include "EnemySpawnManager.h"
#include "InputManager.h"

void ButiEngine::WaveManager::OnUpdate()
{
	MoveWave();

	if (m_isWaveTime)
	{
		//ウェーブ中

		if (InputManager::IsTriggerPauseKey())
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

	m_waveNum = 0;
	m_maxWaveNum = 20;
	m_clearAnimationTime = 0;
	m_gameOverAnimationTime = 0;
	m_isWaveTime = false;
	m_isPopupSpawn = false;
	m_isLastWaveClear = false;
	m_isGameOver = false;
	m_isNextSceneButton = false;
	m_isGameOverButton = false;

	m_nextWaveCount = 0;
}

void ButiEngine::WaveManager::OnShowUI()
{
	GUI::BulletText("WaveNum");
	GUI::InputInt("##waveNum", m_waveNum);
}

void ButiEngine::WaveManager::WaveStart()
{
	m_isWaveTime = true;
}

bool ButiEngine::WaveManager::IsClearAnimationFlag()
{
	return m_isLastWaveClear;
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

	
	//仮でボタンを5回押したらウェーブをクリアできるようにしている
	if (InputManager::IsTriggerCancelKey() && m_isWaveTime)
	{
		m_nextWaveCount++;
	}
	//フィールド内の敵をすべて倒していたらに変える
	if (m_nextWaveCount >= 5)
	{
		m_nextWaveCount = 0;
	
		WaveFinish();
	}
}

void ButiEngine::WaveManager::SpawnEnemy()
{
	//ウェーブ番号に応じて出現させる敵のパターンや配置を変える
	//何ステージ目・何ウェーブ目・敵の名前・位置

	auto sceneName = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneInformation()->GetSceneName();
	std::int16_t underScoreIndex = sceneName.find("_");
	auto size = sceneName.size();
	std::string stageNum = sceneName.substr(underScoreIndex + 1, size);

	std::string fileName = "EnemyData/1_" /*+ stageNum + "_"*/ + std::to_string(m_waveNum) + ".enemyData";

	std::vector<EnemySpawnData> vec_enemySpawnDatas;
	InputCereal(vec_enemySpawnDatas, fileName);

	for (auto enemyData : vec_enemySpawnDatas)
	{
		auto enemy = GetManager().lock()->AddObjectFromCereal(enemyData.m_enemyName);
		auto transformData = enemyData.m_vlp_enemyTransform;
		enemy.lock()->transform->SetLocalPosition(transformData->GetLocalPosition());
		enemy.lock()->transform->SetLocalRotation(transformData->GetLocalRotation());
	}
	
}

void ButiEngine::WaveManager::WaveFinish()
{
	//ウェーブ終了
	m_isWaveTime = false;
	m_isPopupSpawn = true;
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

	//仮
	if (m_gameOverAnimationTime < 120)
	{
		m_gameOverAnimationTime++;
	}
	else
	{
		m_isGameOverButton = true;
	}

	//ウェーブの途中からやり直す
	if (InputManager::IsTriggerDecideKey() && m_isGameOverButton)
	{
		m_waveNum--;
		m_isGameOver = false;
		WaveFinish();
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::WaveManager::Clone()
{
	return ObjectFactory::Create<WaveManager>();
}
