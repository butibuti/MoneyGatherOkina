#include "stdafx_u.h"
#include "EnemySpawner.h"
#include "WaveManager.h"

void ButiEngine::EnemySpawner::OnUpdate()
{
	//更新処理に入ったタイミングで一度だけ通したい処理
	if (!m_isOnce)
	{
		m_isOnce = true;
		OnceUpdate();
	}

	//出現開始まで待機
	if (!m_vlp_waitTimer->Update_continue())
	{
		return;
	}

	//出現間隔を時間経過に応じて短くしていく
	if(m_vlp_subWaitTimer->Update())
	{
		if (m_spawnRate > m_maxSpawnRate)
		{
			m_spawnRate--;
			m_vlp_spawnTimer->ChangeCountFrame(m_spawnRate);
		}	
	}

	//一定時間たったら敵をスポーンさせる
	if (m_vlp_spawnTimer->Update())
	{
		SpawnEnemy();
	}
}

void ButiEngine::EnemySpawner::OnSet()
{
	m_vlp_spawnTimer = ObjectFactory::Create<RelativeTimer>();
	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>();
	m_vlp_subWaitTimer = ObjectFactory::Create<RelativeTimer>();
}

void ButiEngine::EnemySpawner::Start()
{
	m_vlp_spawnTimer->Start();
	m_vlp_waitTimer->Start();
	m_vlp_subWaitTimer->Start();
	m_vlp_subWaitTimer->ChangeCountFrame(10);
	m_waveManagerComponent = GetManager().lock()->GetGameObject("WaveManager").lock()->GetGameComponent<WaveManager>();
	m_spawnRate = 0;
	m_maxSpawnRate = 0;
	m_isOnce = false;
}

void ButiEngine::EnemySpawner::OnShowUI()
{
	switch (m_spawnType)
	{
	case 0: //ハエ
		GUI::BulletText("FlySpawner");
		break;
	case 1: //ストーカー
		GUI::BulletText("StalkerSpawner");
		break;
	case 2: //キバ
		GUI::BulletText("KibaSpawner");
		break;
	case 3: //カザン
		GUI::BulletText("VolcanoSpawner");
		break;
	default:
		break;
	}
	
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::EnemySpawner::Clone()
{
	return ObjectFactory::Create<EnemySpawner>();
}

void ButiEngine::EnemySpawner::SetType(const std::int8_t arg_num)
{
	m_spawnType = arg_num;
}

void ButiEngine::EnemySpawner::OnceUpdate()
{
	//出現し始めるまでにどのくらい待つか
	std::int32_t waitFrame = 1;

	switch (m_spawnType)
	{
	case 0: //ハエ
		m_spawnRate = 360;
		m_maxSpawnRate = 360;
		waitFrame = 60;
		break;
	case 1: //ストーカー
		m_spawnRate = 1200;
		m_maxSpawnRate = 960;
		waitFrame = 1020;
		break;
	case 2: //キバ
		m_spawnRate = 900;
		m_maxSpawnRate = 780;
		waitFrame = 1920;
		break;
	case 3: //カザン
		m_spawnRate = 1200;
		m_maxSpawnRate = 1020;
		waitFrame = 3000;
		break;
	default:
		m_spawnRate = 360;
		m_maxSpawnRate = 360;
		waitFrame = 1;
		break;
	}

	m_vlp_spawnTimer->ChangeCountFrame(m_spawnRate);
	m_vlp_waitTimer->ChangeCountFrame(waitFrame);
}

void ButiEngine::EnemySpawner::SpawnEnemy()
{
	//〇体出現していたらスポーンさせない
	if (m_waveManagerComponent.lock()->GetSpawnCount() >= m_waveManagerComponent.lock()->GetMaxEnemyCount()) { return; }

	//敵を追加したらカウントを増やす
	m_waveManagerComponent.lock()->AddSpawnCount();

	//出現位置等を決める(ランダム)
	Vector3 randomPosition = Vector3(0, 0, 0);
	randomPosition.x = (float)ButiRandom::GetInt(-100, 100);
	randomPosition.z = (float)ButiRandom::GetInt(-100, 100);
	randomPosition.Normalize();
	randomPosition *= (float)ButiRandom::GetInt(1, 20);

	float randomRotateY = 0;
	randomRotateY = (float)ButiRandom::GetInt(-180, 180);

	Value_weak_ptr<GameObject> enemy;

	//スポーンさせる
	switch (m_spawnType)
	{
	case 0: //ハエ
		enemy = GetManager().lock()->AddObjectFromCereal("Enemy_Fly");
		break;
	case 1: //ストーカー
		enemy = GetManager().lock()->AddObjectFromCereal("Enemy_Stalker");
		break;
	case 2: //キバ
		enemy = GetManager().lock()->AddObjectFromCereal("Enemy_Kiba");
		break;
	case 3: //カザン
		enemy = GetManager().lock()->AddObjectFromCereal("Enemy_Volcano");
		break;
	default:
		break;
	}

	enemy.lock()->transform->SetLocalPosition(randomPosition);
	enemy.lock()->transform->SetLocalRotationY_Degrees(randomRotateY);
}
