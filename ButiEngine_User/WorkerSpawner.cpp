#include "stdafx_u.h"
#include "WorkerSpawner.h"
#include "Player.h"

void ButiEngine::WorkerSpawner::OnUpdate()
{
	if (m_vlp_spawnTimer->Update())
	{
		SpawnWorker();
	}
}

void ButiEngine::WorkerSpawner::OnSet()
{
}

void ButiEngine::WorkerSpawner::OnShowUI()
{
	GUI::BulletText("IntervalFrame");
	GUI::DragInt("##interv", &m_spawnIntervalFrame, 1, 0, 300);
	if (GUI::Button("SetInterval"))
	{
		m_vlp_spawnTimer->Reset();
		m_vlp_spawnTimer->ChangeCountFrame(m_spawnIntervalFrame);
	}
}

void ButiEngine::WorkerSpawner::Start()
{
	m_spawnIntervalFrame = 6;
	m_vlp_spawnTimer = ObjectFactory::Create<RelativeTimer>(m_spawnIntervalFrame);
	m_vlp_spawnTimer->Start();

	auto player = GetManager().lock()->GetGameObject(GameObjectTag("Player"));
	auto playerComponent = player.lock()->GetGameComponent<Player>();

	std::uint8_t maxWorkerCount = playerComponent->GetMaxWorkerCount();

	for (std::uint8_t i = 0; i < maxWorkerCount; i++)
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
	gameObject.lock()->transform->SetLocalPosition(playerPos + gameObject.lock()->transform->GetFront() * ButiRandom::GetRandom(10, 20));

	auto worker = GetManager().lock()->AddObjectFromCereal("Worker");
	worker.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition());
}
