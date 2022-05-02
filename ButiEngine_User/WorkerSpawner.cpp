#include "stdafx_u.h"
#include "WorkerSpawner.h"

void ButiEngine::WorkerSpawner::OnUpdate()
{
	if (GameDevice::GetInput()->TriggerKey(Keys::P))
	{
		Vector3 playerPos = m_vwp_player.lock()->transform->GetLocalPosition();
		gameObject.lock()->transform->RollLocalRotationY_Degrees(ButiRandom::GetRandom(0, 360));
		gameObject.lock()->transform->SetLocalPosition(playerPos + gameObject.lock()->transform->GetFront() * ButiRandom::GetRandom(10, 30));

		auto worker = GetManager().lock()->AddObjectFromCereal("Worker", nullptr);
		worker.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition());
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

	for (std::uint8_t i = 0; i < 1; i++)
	{
		Vector3 playerPos = m_vwp_player.lock()->transform->GetLocalPosition();
		gameObject.lock()->transform->RollLocalRotationY_Degrees(ButiRandom::GetRandom(0, 360));
		gameObject.lock()->transform->SetLocalPosition(playerPos + gameObject.lock()->transform->GetFront() * ButiRandom::GetRandom(5, 10));

		auto worker = GetManager().lock()->AddObjectFromCereal("Worker", nullptr);
		worker.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition());
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::WorkerSpawner::Clone()
{
	return ObjectFactory::Create<WorkerSpawner>();
}
