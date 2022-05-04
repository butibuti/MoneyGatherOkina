#include "stdafx_u.h"
#include "WorkerSpawner.h"

void ButiEngine::WorkerSpawner::OnUpdate()
{
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
	if (m_vwp_player.lock() == nullptr)
	{
		m_vwp_player = GetManager().lock()->GetGameObject("Player");
	}

	for (std::uint8_t i = 0; i < 20; i++)
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
	Vector3 playerPos = m_vwp_player.lock()->transform->GetLocalPosition();
	gameObject.lock()->transform->RollLocalRotationY_Degrees(ButiRandom::GetRandom(0, 360));
	gameObject.lock()->transform->SetLocalPosition(playerPos + gameObject.lock()->transform->GetFront() * ButiRandom::GetRandom(5, 10));

	auto worker = GetManager().lock()->AddObjectFromCereal("Worker");
	worker.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition());
}
