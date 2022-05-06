#include "stdafx_u.h"
#include "WaveManager.h"
#include "StartPopUpComponent.h"
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
    //if (ポップ->プレイヤーが触れたかどうか)
	if (m_vwp_startPopUpObject.lock()->IsHitPlayerFlag() && !m_waveTimeFlag)
	//if (InputManager::IsTriggerDecideKey())
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
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::WaveManager::Clone()
{
	return ObjectFactory::Create<WaveManager>();
}
