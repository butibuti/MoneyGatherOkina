#include "stdafx_u.h"
#include "EnemySpawner.h"
#include "EnemySpawnManager.h"
#include "WaveManager.h"
#include "EnemySpawnPointComponent.h"

void ButiEngine::EnemySpawner::OnUpdate()
{
	//更新処理に入ったタイミングで一度だけ通したい処理
	if (!m_isOnce)
	{
		m_isOnce = true;
		OnceUpdate();
	}

	//出現開始まで待機
	if (!m_vlp_waitTimer->Update_continue() || m_startWaitFrame < 0)
	{
		return;
	}

	//出現間隔を時間経過に応じて短くしていく
	if (m_currentMaxSpawnFrame > m_endMaxSpawnFrame)
	{
		m_currentMaxSpawnFrame -= m_reachShorteningMaxFrame * GameDevice::WorldSpeed;
	}
	else
	{
		m_currentMaxSpawnFrame = m_endMaxSpawnFrame;
	}
	if (m_currentMinSpawnFrame > m_endMinSpawnFrame)
	{
		m_currentMinSpawnFrame -= m_reachShorteningMinFrame * GameDevice::WorldSpeed;
	}
	else
	{
		m_currentMinSpawnFrame = m_endMinSpawnFrame;
	}

	//一定時間たったら敵をスポーンさせる
	if (m_vlp_spawnTimer->Update())
	{
		SetRandomSpawnFrame();
		SpawnEnemy();
	}
}

void ButiEngine::EnemySpawner::OnSet()
{
	m_vlp_spawnTimer = ObjectFactory::Create<RelativeTimer>();
	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>();
	m_stageNumber = "0";
	m_spawnType = 0;
}

void ButiEngine::EnemySpawner::Start()
{
	m_vlp_spawnTimer->Start();
	m_vlp_waitTimer->Start();
	m_waveManagerComponent = GetManager().lock()->GetGameObject("WaveManager").lock()->GetGameComponent<WaveManager>();
	m_currentMaxSpawnFrame = 0;
	m_currentMinSpawnFrame = 0;
	m_startMaxSpawnFrame = 400;
	m_endMaxSpawnFrame = 300;
	m_startMinSpawnFrame = 300;
	m_endMinSpawnFrame = 200;
	m_startWaitFrame = 200;
	m_lastIntervalReachFrame = 3000;
	m_reachShorteningMaxFrame = 0;
	m_reachShorteningMinFrame = 0;
	m_randomSpawnFrame = 0;
	m_isOnce = false;

	switch (m_spawnType)
	{
	case 0: //ハエ
		m_maxEnemyFieldCount = 7;
		m_tag = "Fly";
		break;
	case 1: //ストーカー
		m_maxEnemyFieldCount = 4;
		m_tag = "Stalker";
		break;
	case 2: //キバ
		m_maxEnemyFieldCount = 3;
		m_tag = "Kiba";
		break;
	case 3: //カザン
		m_maxEnemyFieldCount = 2;
		m_tag = "Volcano";
		break;
	}
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

	GUI::BulletText("MaxFieldSpawnCount");
	GUI::InputInt("##maxFieldCount", m_maxEnemyFieldCount);
	
	GUI::BulletText("CurrentMaxSpawnFrame");
	GUI::BulletText(m_currentMaxSpawnFrame);
	GUI::BulletText("CurrentMinSpawnFrame");
	GUI::BulletText(m_currentMinSpawnFrame);

	GUI::BulletText("StartWaitFrame");
	if (GUI::DragFloat("##StartWaitFrame", m_startWaitFrame, 10,-10, 12000))
	{
		if (m_startWaitFrame > 0)
		{
			m_vlp_waitTimer->ChangeCountFrame(m_startWaitFrame);
		}
		FixShorteningFrame();
	}

	GUI::BulletText("StartMaxSpawnFrame");
	if (GUI::DragFloat("##StartMaxSpawnFrame", m_startMaxSpawnFrame, 10,0, 12000))
	{
		m_vlp_waitTimer->ChangeCountFrame(m_startMaxSpawnFrame);
		FixShorteningFrame();
	}
	GUI::BulletText("EndMaxSpawnFrame");
	if (GUI::DragFloat("##EndMaxSpawnFrame", m_endMaxSpawnFrame, 10,0, 12000))
	{
		m_vlp_waitTimer->ChangeCountFrame(m_endMaxSpawnFrame);
		FixShorteningFrame();
	}

	GUI::BulletText("StartMinSpawnFrame");
	if (GUI::DragFloat("##StartMinSpawnFrame", m_startMinSpawnFrame, 10,0, 12000))
	{
		m_vlp_waitTimer->ChangeCountFrame(m_startMinSpawnFrame);
		FixShorteningFrame();
	}
	GUI::BulletText("EndMinSpawnFrame");
	if (GUI::DragFloat("##EndMinSpawnFrame", m_endMinSpawnFrame, 10,0, 12000))
	{
		m_vlp_waitTimer->ChangeCountFrame(m_endMinSpawnFrame);
		FixShorteningFrame();
	}

	GUI::BulletText("LastIntervalReachFrame");
	if (GUI::DragFloat("##LastIntervalReachFrame", m_lastIntervalReachFrame, 10,0, 12000))
	{
		FixShorteningFrame();
	}

	GUI::BulletText("StageNum(1-3)");
	GUI::InputInt("##StageData", m_inputStageNumber);

	if (GUI::Button("OutputData"))
	{
		EnemySpawnData outputDatas;
		outputDatas.m_startWaitFrame = m_startWaitFrame;
		outputDatas.m_startMaxSpawnFrame = m_startMaxSpawnFrame;
		outputDatas.m_endMaxSpawnFrame = m_endMaxSpawnFrame;
		outputDatas.m_startMinSpawnFrame = m_startMinSpawnFrame;
		outputDatas.m_endMinSpawnFrame = m_endMinSpawnFrame;
		outputDatas.m_lastIntervalReachFrame = m_lastIntervalReachFrame;

		std::string outputFileName = "EnemyData/" + std::to_string(m_spawnType) + "_" + std::to_string(m_inputStageNumber) + ".enemyData";
		OutputCereal(outputDatas, outputFileName);
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::EnemySpawner::Clone()
{
	return ObjectFactory::Create<EnemySpawner>();
}

void ButiEngine::EnemySpawner::OnceUpdate()
{
	std::string filePath = "Resources/";
	std::string fileName = "EnemyData/" + std::to_string(m_spawnType) + "_" + m_stageNumber + ".enemyData";
	EnemySpawnData vec_enemySpawnDatas;

	if (Util::ExistFile(filePath + fileName))
	{
		InputCereal(vec_enemySpawnDatas, fileName);	
		m_startWaitFrame = vec_enemySpawnDatas.m_startWaitFrame;
		m_startMaxSpawnFrame = vec_enemySpawnDatas.m_startMaxSpawnFrame;
		m_endMaxSpawnFrame = vec_enemySpawnDatas.m_endMaxSpawnFrame;
		m_startMinSpawnFrame = vec_enemySpawnDatas.m_startMinSpawnFrame;
		m_endMinSpawnFrame = vec_enemySpawnDatas.m_endMinSpawnFrame;
		m_lastIntervalReachFrame = vec_enemySpawnDatas.m_lastIntervalReachFrame;
	}

	FixShorteningFrame();

	m_currentMaxSpawnFrame = m_startMaxSpawnFrame;
	m_currentMinSpawnFrame = m_startMinSpawnFrame;

	SetRandomSpawnFrame();

	m_vlp_waitTimer->ChangeCountFrame(m_startWaitFrame);
}

void ButiEngine::EnemySpawner::SpawnEnemy()
{
	//ステージクリア上限数出現していたらスポーンさせない
	if (m_waveManagerComponent.lock()->GetSpawnCount() >= m_waveManagerComponent.lock()->GetMaxEnemyCount()) { return; }
	
	auto objects = GetManager().lock()->GetGameObjects(GameObjectTag(m_tag));
	std::int32_t objectsCount = objects.size();
	
	//〇体以上フィールドにいる場合は出現させない
	if (objectsCount >= m_maxEnemyFieldCount) { return; }

	//敵を追加したらカウントを増やす
	m_waveManagerComponent.lock()->AddSpawnCount();

	//出現位置等を決める(ランダム)
	auto vec_enemyGameObject = GetManager().lock()->GetGameObjects(GameObjectTag("Enemy"));

	Vector3 randomPosition = Vector3(0, 0, 0);
	float radius = 8;
	auto player = GetManager().lock()->GetGameObject("Player");
	Vector3 playerPosition = player.lock()->transform->GetLocalPosition();

	//8回まで比較する
	for (std::int8_t i = 0; i < 8; i++)
	{
		bool isSucsess = true;
		randomPosition.x = (float)ButiRandom::GetInt(-100, 100);
		randomPosition.z = (float)ButiRandom::GetInt(-100, 100);
		randomPosition.Normalize();
		randomPosition *= (float)ButiRandom::GetInt(1, 20);
		float distance;
		auto end = vec_enemyGameObject.end();
		for (auto itr = vec_enemyGameObject.begin(); itr != end; ++itr)
		{
			auto enemyPosition = (*itr)->transform->GetLocalPosition();
			distance = randomPosition.Distance(enemyPosition);
			if (distance < radius)
			{
				isSucsess = false;
			}
		}
		distance = randomPosition.Distance(playerPosition);
		if (distance < radius)
		{
			isSucsess = false;
		}

		if (isSucsess)
		{
			break;
		}
	}

	auto spawnPoint = GetManager().lock()->AddObjectFromCereal("EnemySpawnPoint");
	auto spawnPointComponent = spawnPoint.lock()->GetGameComponent<EnemySpawnPointComponent>();
	spawnPointComponent->SetType(m_spawnType);
	spawnPointComponent->SetPosition(randomPosition);
}

void ButiEngine::EnemySpawner::FixShorteningFrame()
{
	m_reachShorteningMaxFrame = (m_startMaxSpawnFrame - m_endMaxSpawnFrame) / m_lastIntervalReachFrame;
	m_reachShorteningMinFrame = (m_startMinSpawnFrame - m_endMinSpawnFrame) / m_lastIntervalReachFrame;
}

void ButiEngine::EnemySpawner::SetRandomSpawnFrame()
{
	m_randomSpawnFrame = ButiRandom::GetInt(m_currentMinSpawnFrame, m_currentMaxSpawnFrame);
	m_vlp_spawnTimer->ChangeCountFrame(m_randomSpawnFrame);
}

void ButiEngine::EnemySpawner::SpawnAnimation()
{
}
