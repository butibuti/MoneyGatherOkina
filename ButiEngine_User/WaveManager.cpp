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
	}
	else
	{
		//ウェーブ外
	}
}

void ButiEngine::WaveManager::OnSet()
{
}

void ButiEngine::WaveManager::Start()
{
	m_vwp_startPopUpObject = GetManager().lock()->GetGameObject("StartPopUpObject").lock()->GetGameComponent<StartPopUpComponent>();

	m_waveNum = 1;
	m_maxWaveNum = 20;
	m_waveTimeFlag = false;

	m_nextWaveCount = 0;
}

void ButiEngine::WaveManager::OnShowUI()
{
}

void ButiEngine::WaveManager::WaveStart()
{
	m_waveTimeFlag = true;
}

void ButiEngine::WaveManager::FixWaveNum()
{
	//Wave番号を範囲内に修正
	if (m_waveNum < 1)
	{
		m_waveNum = 1;
	}
	else if (m_waveNum > m_maxWaveNum)
	{
		m_waveNum = m_maxWaveNum;
	}
}

void ButiEngine::WaveManager::MoveWave()
{
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

	if (InputManager::IsTriggerCancelKey() && m_waveTimeFlag)
	{
		m_nextWaveCount++;
	}
	if (m_nextWaveCount >= 5)
	{
		m_nextWaveCount = 0;

		//ウェーブ終了
		m_waveTimeFlag = false;

		//ポップをスポーンさせる
		m_vwp_startPopUpObject.lock()->AppearPopUp();
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

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::WaveManager::Clone()
{
	return ObjectFactory::Create<WaveManager>();
}
