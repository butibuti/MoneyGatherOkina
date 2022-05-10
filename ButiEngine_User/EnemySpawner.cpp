#include "stdafx_u.h"
#include "EnemySpawner.h"

void ButiEngine::EnemySpawner::OnUpdate()
{
	if (!m_isOnce)
	{
		m_isOnce = true;
		switch (m_spawnType)
		{
		case 0: //ハエ
			m_spawnRate = 300;
			m_maxSpawnRate = 240;
			m_spawnStartTimer = 0;
			break;
		case 1: //ストーカー
			m_spawnRate = 420;
			m_maxSpawnRate = 300;
			m_spawnStartTimer = 900;
			break;
		case 2: //キバ
			m_spawnRate = 540;
			m_maxSpawnRate = 360;
			m_spawnStartTimer = 1800;
			break;
		case 3: //カザン
			m_spawnRate = 660;
			m_maxSpawnRate = 420;
			m_spawnStartTimer = 2700;
			break;
		default:
			break;
		}
	}

	if (m_spawnStartTimer > 0)
	{
		m_spawnStartTimer--;
		return;
	}
	if (m_subWaitTimer < 5)
	{
		m_subWaitTimer++;
	}
	else
	{
		m_subWaitTimer = 0;
		if (m_spawnRate > m_maxSpawnRate)
		{
			m_spawnRate--;
		}		
	}

	if (m_spawnTimer < m_spawnRate)
	{
		m_spawnTimer++;
	}
	else
	{
		m_randomPosition.x = (float)ButiRandom::GetInt(-20, 20);
		m_randomPosition.z = (float)ButiRandom::GetInt(-20, 20);

		m_randomPosition.Normalize();
		m_randomPosition *= (float)ButiRandom::GetInt(1, 20);


		m_spawnTimer = 0;

		Value_weak_ptr<GameObject> enemy;

		//スポーンさせる
		switch (m_spawnType)
		{
		case 0: //ハエ
			enemy = GetManager().lock()->AddObjectFromCereal("Enemy_Flie");
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

		enemy.lock()->transform->SetLocalPosition(m_randomPosition);
	}
}

void ButiEngine::EnemySpawner::OnSet()
{
}

void ButiEngine::EnemySpawner::Start()
{
	m_randomPosition = Vector3(0, 0, 0);
	m_spawnTimer = 0;
	m_spawnRate = 0;
	m_maxSpawnRate = 0;
	m_spawnStartTimer = 0;
	m_subWaitTimer = 0;
	m_isOnce = false;
}

void ButiEngine::EnemySpawner::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::EnemySpawner::Clone()
{
	return ObjectFactory::Create<EnemySpawner>();
}

void ButiEngine::EnemySpawner::SetType(const std::int8_t arg_num)
{
	m_spawnType = arg_num;
}
