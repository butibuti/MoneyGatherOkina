#include "stdafx_u.h"
#include "WorkerSpawner.h"
#include "Player.h"
#include "Worker.h"
#include "WaveManager.h"

void ButiEngine::WorkerSpawner::OnUpdate()
{
	if (m_isSpawnWorker)
	{
		SpawnWorker();
	}

	if (m_isRandomSpawnWorker)
	{
		RandomSpawnWorker();
	}
}

void ButiEngine::WorkerSpawner::OnSet()
{
}

void ButiEngine::WorkerSpawner::OnShowUI()
{
}

void ButiEngine::WorkerSpawner::Start()
{
	m_isTutorial = GetManager().lock()->GetGameObject("WaveManager").lock()->GetGameComponent<WaveManager>()->IsTutorial();

	m_spawnIntervalFrame = 2;
	m_vlp_spawnTimer = ObjectFactory::Create<RelativeTimer>(m_spawnIntervalFrame);
	m_vlp_randomSpawnTimer = ObjectFactory::Create<RelativeTimer>(6);

	auto player = GetManager().lock()->GetGameObject(GameObjectTag("Player"));
	auto playerComponent = player.lock()->GetGameComponent<Player>();

	m_initSpawnWorkerCount = playerComponent->GetMaxWorkerCount();
	if (m_isTutorial)
	{
		m_initSpawnWorkerCount = 0;
	}

	m_isSpawnWorker = false;
	m_isRandomSpawnWorker = false;
	m_isSpawned = false;

	SetWorkerPosition();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::WorkerSpawner::Clone()
{
	return ObjectFactory::Create<WorkerSpawner>();
}

void ButiEngine::WorkerSpawner::RandomSpawnWorker()
{
	auto player = GetManager().lock()->GetGameObject(GameObjectTag("Player"));
	auto playerComponent = player.lock()->GetGameComponent<Player>();

	if (m_vlp_randomSpawnTimer->Update())
	{
		//Playerの周りにランダムにスポーン
		Vector3 playerPos = player.lock()->transform->GetLocalPosition();
		gameObject.lock()->transform->RollLocalRotationY_Degrees(ButiRandom::GetRandom(0, 360));
		gameObject.lock()->transform->SetLocalPosition(playerPos + gameObject.lock()->transform->GetFront() * ButiRandom::GetRandom(5, 10));

		auto worker = GetManager().lock()->AddObjectFromCereal("Worker");
		worker.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition());

		//Workerが上限いたらスポーン止める
		std::uint8_t maxWorkerCount = playerComponent->GetMaxWorkerCount();
		std::uint8_t workerCount = GetManager().lock()->GetGameObjects(GameObjectTag("Worker")).size();

		if (workerCount == maxWorkerCount)
		{
			m_isSpawnWorker = false;
			m_vlp_randomSpawnTimer->Stop();
		}
	}
}

void ButiEngine::WorkerSpawner::SpawnWorker()
{
	if (m_isSpawned) { return; }
	if (m_vec_workers.size() == 0)
	{
		m_isSpawned = true;
		m_vlp_spawnTimer->Stop();
		m_vlp_spawnTimer->Reset();
	}

	if (m_vlp_spawnTimer->Update())
	{
		std::uint8_t index = ButiRandom::GetInt(0, m_vec_workers.size() - 1);
		auto worker = m_vec_workers[index];
		worker.lock()->Spawn();
		m_vec_workers.erase(m_vec_workers.begin() + index);

		if (m_vec_workers.size() == 0)
		{
			m_isSpawned = true;
			m_vlp_spawnTimer->Stop();
			m_vlp_spawnTimer->Reset();
		}
	}
}

void ButiEngine::WorkerSpawner::SetWorkerPosition()
{
	if (m_isTutorial) { return; }

	auto player = GetManager().lock()->GetGameObject(GameObjectTag("Player"));

	float radius = 1.5f;

	auto workerCenter = player.lock()->transform->Clone();
	Vector3 centerScale = 1.0f;
	workerCenter->SetLocalScale(centerScale);
	auto workerTransform = ObjectFactory::Create<Transform>();
	workerTransform->SetBaseTransform(workerCenter);
	workerTransform->SetLocalPosition(Vector3(radius, 0.0f, 0.0f));

	std::uint8_t vertexCount = ButiRandom::GetInt(3, 10);
	float rollAngle = 360.0f / vertexCount;

	std::uint8_t dontSpawnWorkerCount = m_initSpawnWorkerCount;

	while (dontSpawnWorkerCount > vertexCount)
	{
		for (std::uint8_t i = 0; i < vertexCount; i++)
		{
			auto worker = GetManager().lock()->AddObjectFromCereal("Worker");

			Vector3 pos = workerTransform->GetWorldPosition();
			worker.lock()->transform->SetWorldPosition(pos);

			m_vec_workers.push_back(worker.lock()->GetGameComponent<Worker>());

			dontSpawnWorkerCount--;

			workerCenter->RollLocalRotationY_Degrees(rollAngle);
		}

		centerScale += 0.75f;
		workerCenter->SetLocalScale(centerScale);
		workerCenter->RollLocalRotationY_Degrees(90.0f);
	}

	rollAngle = 360.0f / dontSpawnWorkerCount;

	std::uint8_t loopCount = dontSpawnWorkerCount;
	for (std::uint8_t i = 0; i < loopCount; i++)
	{
		auto worker = GetManager().lock()->AddObjectFromCereal("Worker");

		Vector3 pos = workerTransform->GetWorldPosition();
		worker.lock()->transform->SetWorldPosition(pos);

		m_vec_workers.push_back(worker.lock()->GetGameComponent<Worker>());

		dontSpawnWorkerCount--;

		workerCenter->RollLocalRotationY_Degrees(rollAngle);
	}
}
