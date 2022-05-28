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

	if (m_isHeatUpActive)
	{
		if (m_vlp_changeTimer->Update())
		{
			m_isHeatUp = !m_isHeatUp;
		}
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
	m_vlp_changeTimer = ObjectFactory::Create<RelativeTimer>();
	m_stageNumber = "0";
	m_spawnType = 0;
	m_spawnerIndex = 0;
}

void ButiEngine::EnemySpawner::Start()
{
	m_vlp_spawnTimer->Start();
	m_vlp_waitTimer->Start();
	m_vlp_changeTimer->Start();
	m_waveManagerComponent = GetManager().lock()->GetGameObject("WaveManager").lock()->GetGameComponent<WaveManager>();
	m_maxEnemyFieldCount = 0;
	m_maxHeatEnemyFieldCount = 0;
	m_currentMaxSpawnFrame = 0;
	m_currentMinSpawnFrame = 0;
	m_startMaxSpawnFrame = 400;
	m_endMaxSpawnFrame = 300;
	m_startMinSpawnFrame = 300;
	m_endMinSpawnFrame = 200;
	m_startWaitFrame = 300;
	m_lastIntervalReachFrame = 3000;
	m_reachShorteningMaxFrame = 0;
	m_reachShorteningMinFrame = 0;
	m_randomSpawnFrame = 0;
	m_maxSpawnRate = 1;
	m_minSpawnRate = 1;
	m_changeInterval = 300;
	m_isOnce = false;
	m_isHeatUp = false;
	m_isHeatUpActive = false;

	switch (m_spawnType)
	{
	case 0: //ハエ
		m_tag = "Fly_" + std::to_string(m_spawnerIndex);
		break;
	case 1: //ストーカー
		m_tag = "Stalker_" + std::to_string(m_spawnerIndex);
		break;
	case 2: //キバ
		m_tag = "Kiba_" + std::to_string(m_spawnerIndex);
		break;
	case 3: //カザン
		m_tag = "Volcano_" + std::to_string(m_spawnerIndex);
		break;
	}
}

void ButiEngine::EnemySpawner::OnShowUI()
{
	switch (m_spawnType)
	{
	case 0: //ハエ
		GUI::BulletText("FlySpawner_" + std::to_string(m_spawnerIndex));
		break;
	case 1: //ストーカー
		GUI::BulletText("StalkerSpawner_" + std::to_string(m_spawnerIndex));
		break;
	case 2: //キバ
		GUI::BulletText("KibaSpawner_" + std::to_string(m_spawnerIndex));
		break;
	case 3: //カザン
		GUI::BulletText("VolcanoSpawner_" + std::to_string(m_spawnerIndex));
		break;
	default:
		break;
	}

	GUI::Text("------CurrentParam------");
	GUI::Text("CurrentMaxSpawnFrame:%f", m_currentMaxSpawnFrame);
	GUI::Text("CurrentMinSpawnFrame:%f", m_currentMinSpawnFrame);
	GUI::BulletText("IsHeatUp_Current");
	GUI::Checkbox("##IsHeatUp_Current", m_isHeatUp);

	GUI::Text("------Common------");
	GUI::BulletText("StageNum(0-1)");
	GUI::InputInt("##StageData", m_inputStageNumber);

	GUI::Text("------EnemySpawnData------");
	GUI::BulletText("MaxFieldSpawnCount");
	GUI::InputInt("##maxFieldCount", m_maxEnemyFieldCount);

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
		FixShorteningFrame();
	}
	GUI::BulletText("EndMaxSpawnFrame");
	if (GUI::DragFloat("##EndMaxSpawnFrame", m_endMaxSpawnFrame, 10,0, 12000))
	{
		FixShorteningFrame();
	}

	GUI::BulletText("StartMinSpawnFrame");
	if (GUI::DragFloat("##StartMinSpawnFrame", m_startMinSpawnFrame, 10,0, 12000))
	{
		FixShorteningFrame();
	}
	GUI::BulletText("EndMinSpawnFrame");
	if (GUI::DragFloat("##EndMinSpawnFrame", m_endMinSpawnFrame, 10,0, 12000))
	{
		FixShorteningFrame();
	}

	GUI::BulletText("LastIntervalReachFrame");
	if (GUI::DragFloat("##LastIntervalReachFrame", m_lastIntervalReachFrame, 10,0, 12000))
	{
		FixShorteningFrame();
	}

	if (GUI::Button("Output_EnemyData"))
	{
		EnemySpawnData outputDatas;
		outputDatas.m_startWaitFrame = m_startWaitFrame;
		outputDatas.m_startMaxSpawnFrame = m_startMaxSpawnFrame;
		outputDatas.m_endMaxSpawnFrame = m_endMaxSpawnFrame;
		outputDatas.m_startMinSpawnFrame = m_startMinSpawnFrame;
		outputDatas.m_endMinSpawnFrame = m_endMinSpawnFrame;
		outputDatas.m_lastIntervalReachFrame = m_lastIntervalReachFrame;
		outputDatas.m_spawnerIndex = m_spawnerIndex;
		outputDatas.m_maxSpawnCount = m_maxEnemyFieldCount;

		std::string outputFileName = "EnemyData/" + std::to_string(m_spawnType) + "_" + std::to_string(m_inputStageNumber) + "_" + std::to_string(m_spawnerIndex) + ".enemyData";
		OutputCereal(outputDatas, outputFileName);
	}

	GUI::Text("------HeatUpData------");
	GUI::BulletText("IsHeatUp_Data");
	GUI::Checkbox("##IsHeatUp_Data", m_isHeatUpActive);

	GUI::BulletText("MaxHeatUpSpawnCount");
	GUI::InputInt("##MaxHeatUpSpawnCount", m_maxHeatEnemyFieldCount);

	GUI::BulletText("MaxSpawnRate");
	GUI::DragFloat("##MaxSpawnRate", m_maxSpawnRate, 0.02f, 0, 100);

	GUI::BulletText("MinSpawnRate");
	GUI::DragFloat("##MinSpawnRate", m_minSpawnRate, 0.02f, 0, 100);

	GUI::BulletText("ChangeInterval");
	GUI::DragFloat("##ChangeInterval", m_changeInterval, 10, 0, 6000);

	if (GUI::Button("Output_HeatUpData"))
	{
		HeatUpData outputDatas;
		outputDatas.m_isHeatUp = m_isHeatUpActive;
		outputDatas.m_maxHeatUpSpawnCount = m_maxHeatEnemyFieldCount;
		outputDatas.m_maxSpawnRate = m_maxSpawnRate;
		outputDatas.m_minSpawnRate = m_minSpawnRate;
		outputDatas.m_changeInterval = m_changeInterval;

		std::string outputFileName = "EnemyData/" + std::to_string(m_spawnType) + "_" + std::to_string(m_inputStageNumber) + "_" + std::to_string(m_spawnerIndex) + ".heatData";
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
	std::string fileName = "EnemyData/" + std::to_string(m_spawnType) + "_" + m_stageNumber + "_" + std::to_string(m_spawnerIndex) + ".enemyData";
	EnemySpawnData vec_enemySpawnDatas;

	if (Util::ExistFile(filePath + fileName))
	{
		InputCereal(vec_enemySpawnDatas, fileName);	
		m_startWaitFrame = vec_enemySpawnDatas.m_startWaitFrame;
		m_startMaxSpawnFrame = vec_enemySpawnDatas.m_startMaxSpawnFrame;
		m_endMaxSpawnFrame = vec_enemySpawnDatas.m_endMaxSpawnFrame;
		m_startMinSpawnFrame = vec_enemySpawnDatas.m_startMinSpawnFrame;
		m_endMinSpawnFrame = vec_enemySpawnDatas.m_endMinSpawnFrame;
		m_spawnerIndex = vec_enemySpawnDatas.m_spawnerIndex;
		m_lastIntervalReachFrame = vec_enemySpawnDatas.m_lastIntervalReachFrame;
		m_maxEnemyFieldCount = vec_enemySpawnDatas.m_maxSpawnCount;
	}

	filePath = "Resources/";
	fileName = "EnemyData/" + std::to_string(m_spawnType) + "_" + m_stageNumber + "_" + std::to_string(m_spawnerIndex) + ".heatData";
	HeatUpData vec_heatUpDatas;

	if (Util::ExistFile(filePath + fileName))
	{
		InputCereal(vec_heatUpDatas, fileName);
		m_isHeatUpActive = vec_heatUpDatas.m_isHeatUp;
		if (m_isHeatUpActive)
		{
			m_maxSpawnRate = vec_heatUpDatas.m_maxSpawnRate;
			m_minSpawnRate = vec_heatUpDatas.m_minSpawnRate;
			m_changeInterval = vec_heatUpDatas.m_changeInterval;
			m_maxHeatEnemyFieldCount = vec_heatUpDatas.m_maxHeatUpSpawnCount;
		}
	}

	FixShorteningFrame();

	m_currentMaxSpawnFrame = m_startMaxSpawnFrame;
	m_currentMinSpawnFrame = m_startMinSpawnFrame;

	SetRandomSpawnFrame();

	m_vlp_waitTimer->ChangeCountFrame(m_startWaitFrame);
	m_vlp_changeTimer->ChangeCountFrame(m_changeInterval);
}

void ButiEngine::EnemySpawner::SpawnEnemy()
{
	//ステージクリア上限数出現していたらスポーンさせない
	if (m_waveManagerComponent.lock()->GetSpawnCount() >= m_waveManagerComponent.lock()->GetClearPoint()) { return; }
	if (m_waveManagerComponent.lock()->IsClearAnimation()) { return; }

	auto objects = GetManager().lock()->GetGameObjects(GameObjectTag(m_tag));
	std::int32_t objectsCount = objects.size();
	
	//〇体以上フィールドにいる場合は出現させない
	if (m_isHeatUp)
	{
		if (objectsCount >= m_maxHeatEnemyFieldCount) { return; }
	}
	else
	{
		if (objectsCount >= m_maxEnemyFieldCount) { return; }
	}

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
	spawnPointComponent->SetEnemyTag(m_tag);
}

void ButiEngine::EnemySpawner::FixShorteningFrame()
{
	m_reachShorteningMaxFrame = (m_startMaxSpawnFrame - m_endMaxSpawnFrame) / m_lastIntervalReachFrame;
	m_reachShorteningMinFrame = (m_startMinSpawnFrame - m_endMinSpawnFrame) / m_lastIntervalReachFrame;
}

void ButiEngine::EnemySpawner::SetRandomSpawnFrame()
{
	if (m_isHeatUp)
	{
		auto maxRate = 1.0f / m_maxSpawnRate;
		auto minRate = 1.0f / m_minSpawnRate;
		auto calcMaxFrame = m_currentMaxSpawnFrame * maxRate;
		auto calcMinFrame = m_currentMinSpawnFrame * minRate;
		m_randomSpawnFrame = ButiRandom::GetInt(calcMinFrame, calcMaxFrame);
	}
	else
	{
		m_randomSpawnFrame = ButiRandom::GetInt(m_currentMinSpawnFrame, m_currentMaxSpawnFrame);
	}

	m_vlp_spawnTimer->ChangeCountFrame(m_randomSpawnFrame);
}

void ButiEngine::EnemySpawner::SpawnAnimation()
{
}
