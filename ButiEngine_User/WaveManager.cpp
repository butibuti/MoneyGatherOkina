#include "stdafx_u.h"
#include "WaveManager.h"
#include "StartPopUpComponent.h"
#include "EnemySpawnManager.h"
#include "InputManager.h"

void ButiEngine::WaveManager::OnUpdate()
{
	MoveWave();

	if (m_waveTimeFlag)
	{
		//ウェーブ中

		if (InputManager::IsTriggerPauseKey())
		{
			m_gameOverFlag = true;
		}

	}
	else
	{
		//ウェーブ外

		//最終ウェーブをクリアしていたら
		if (m_waveNum >= m_maxWaveNum)
		{
			m_lastWaveClearFlag = true;
		}

		if (m_popupSpawnFlag && !m_lastWaveClearFlag)
		{
			m_popupSpawnFlag = false;
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
	m_waveTimeFlag = false;
	m_popupSpawnFlag = false;
	m_lastWaveClearFlag = false;
	m_gameOverFlag = false;
	m_nextSceneButtonFlag = false;
	m_gameOverButtonFlag = false;

	m_nextWaveCount = 0;
}

void ButiEngine::WaveManager::OnShowUI()
{
	GUI::BulletText("WaveNum");
	GUI::InputInt("##waveNum", m_waveNum);
}

void ButiEngine::WaveManager::WaveStart()
{
	m_waveTimeFlag = true;
}

bool ButiEngine::WaveManager::IsClearAnimationFlag()
{
	return m_lastWaveClearFlag;
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
	if (m_lastWaveClearFlag) return;

	//プレイヤーがポップに触れたらウェーブ開始
	if (m_vwp_startPopUpObject.lock()->IsHitPlayerFlag() && !m_waveTimeFlag)
	{
		//ウェーブ番号を進める
		m_waveNum++;
		FixWaveNum();

		//ウェーブ開始
		m_waveTimeFlag = true;

		//ポップを引っ込める
		m_vwp_startPopUpObject.lock()->DisappearPopUp();

		//敵をスポーンさせる
		SpawnEnemy();
	}

	
	//仮でボタンを5回押したらウェーブをクリアできるようにしている
	if (InputManager::IsTriggerCancelKey() && m_waveTimeFlag)
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
	m_waveTimeFlag = false;
	m_popupSpawnFlag = true;
}

void ButiEngine::WaveManager::StageClearAnimation()
{
	//最終ウェーブクリア後に通るようにする
	if (!m_lastWaveClearFlag) return;

	//仮
	if (m_clearAnimationTime < 120)
	{
		m_clearAnimationTime++;
	}
	else
	{
		m_nextSceneButtonFlag = true;
	}

	//ステージセレクトへ
	if (InputManager::IsTriggerDecideKey() && m_nextSceneButtonFlag)
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
	if (!m_gameOverFlag) return;

	//仮
	if (m_gameOverAnimationTime < 120)
	{
		m_gameOverAnimationTime++;
	}
	else
	{
		m_gameOverButtonFlag = true;
	}

	//ウェーブの途中からやり直す
	if (InputManager::IsTriggerDecideKey() && m_gameOverButtonFlag)
	{
		m_waveNum--;
		m_gameOverFlag = false;
		WaveFinish();
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::WaveManager::Clone()
{
	return ObjectFactory::Create<WaveManager>();
}
