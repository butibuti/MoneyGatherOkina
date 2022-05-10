#include "stdafx_u.h"
#include "WorkerSpawner.h"
#include "Player.h"

void ButiEngine::WorkerSpawner::OnUpdate()
{
	if (m_vlp_spawnTimer->Update())
	{
		SpawnWorker();
	}

	if (GameDevice::GetInput()->TriggerKey(Keys::P))
	{
		SpawnWorker();
	}
}

void ButiEngine::WorkerSpawner::OnSet()
{
}

void ButiEngine::WorkerSpawner::Start()
{
	m_spawnIntervalFrame = 180;
	m_vlp_spawnTimer = ObjectFactory::Create<RelativeTimer>(180);
	m_vlp_spawnTimer->Start();

	for (std::uint8_t i = 0; i < 10; i++)
	{
		SpawnWorker();
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::WorkerSpawner::Clone()
{
	return ObjectFactory::Create<WorkerSpawner>();
}

void ButiEngine::WorkerSpawner::SpawnWorker()
{
	auto player = GetManager().lock()->GetGameObject(GameObjectTag("Player"));
	auto playerComponent = player.lock()->GetGameComponent<Player>();
	
	//Workerが上限以上いたらスポーンさせない
	std::uint8_t maxWorkerCount = playerComponent->GetMaxWorkerCount();
	std::uint8_t workerCount = GetManager().lock()->GetGameObjects(GameObjectTag("Worker")).size();
	if (workerCount >= maxWorkerCount)
	{
		m_vlp_spawnTimer->Stop();
		return; 
	}

	//Playerの周りにランダムにスポーン
	Vector3 playerPos = player.lock()->transform->GetLocalPosition();
	gameObject.lock()->transform->RollLocalRotationY_Degrees(ButiRandom::GetRandom(0, 360));
	gameObject.lock()->transform->SetLocalPosition(playerPos + gameObject.lock()->transform->GetFront() * ButiRandom::GetRandom(5, 10));

	auto worker = GetManager().lock()->AddObjectFromCereal("Worker");
	worker.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition());
}
