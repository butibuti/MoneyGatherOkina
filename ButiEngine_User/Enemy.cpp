#include "stdafx_u.h"
#include "Enemy.h"
#include "Pocket.h"

void ButiEngine::Enemy::OnUpdate()
{
}

void ButiEngine::Enemy::OnSet()
{
	//gameObject.lock()->AddCollisionStayReaction(std::function<void(ButiBullet::ContactData&)>([](ButiBullet::ContactData& arg_other)->void 
	//	{
	//		GUI::Console("Stay");
	//	}));
}

void ButiEngine::Enemy::OnShowUI()
{
	GUI::BulletText("PocketCount");
	GUI::DragInt("##p", m_testPocketCount, 1.0f, 0, 20);
	if (GUI::Button("Create"))
	{
		CreatePocket(m_testPocketCount);
	}
}

void ButiEngine::Enemy::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Enemy::Clone()
{
	return ObjectFactory::Create<Enemy>();
}

void ButiEngine::Enemy::CreatePocket(std::uint8_t arg_pocketCount)
{
	RemoveAllPocket();
	m_pocketCount = arg_pocketCount;

	auto pocketCenter = gameObject.lock()->transform->Clone();
	auto pocketTransform = ObjectFactory::Create<Transform>();
	pocketTransform->SetBaseTransform(pocketCenter);
	pocketTransform->SetLocalPosition(Vector3(0.0f, 0.0f, 0.5f));

	float rollAngle = 360.0f / m_pocketCount;

	for (std::uint8_t i = 0; i < m_pocketCount; i++)
	{
		auto pocket = GetManager().lock()->AddObjectFromCereal("Pocket", ObjectFactory::Create<Transform>());
		pocket.lock()->transform->SetBaseTransform(gameObject.lock()->transform);

		Vector3 pos = pocketTransform->GetWorldPosition();
		pocket.lock()->transform->SetWorldPosition(pos);
		pocket.lock()->transform->SetLocalScale(0.3f);

		m_vec_pockets.push_back(pocket);

		pocketCenter->RollLocalRotationY_Degrees(rollAngle);
	}
}

std::uint8_t ButiEngine::Enemy::GetHasWorkerPocketCount()
{
	std::uint8_t hasWorkerPocketCount = 0;

	auto end = m_vec_pockets.end();
	for (auto itr = m_vec_pockets.begin(); itr != end; ++itr)
	{
		if ((*itr).lock()->GetGameComponent<Pocket>()->HasWorker())
		{
			hasWorkerPocketCount++;
		}
	}

	return hasWorkerPocketCount;
}

void ButiEngine::Enemy::RemoveAllPocket()
{
	auto end = m_vec_pockets.end();
	for (auto itr = m_vec_pockets.begin(); itr != end; ++itr)
	{
		(*itr).lock()->SetIsRemove(true);
	}
	m_vec_pockets.clear();
}
