#include "stdafx_u.h"
#include "Enemy.h"
#include "Pocket.h"

void ButiEngine::Enemy::OnUpdate()
{
}

void ButiEngine::Enemy::OnSet()
{
	m_testPocketCount = 0;
	//gameObject.lock()->AddCollisionStayReaction(std::function<void(ButiBullet::ContactData&)>([](ButiBullet::ContactData& arg_other)->void 
	//	{
	//		int a = 0;
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
	GUI::BulletText("StickWorkerCount:%d", GetExistWorkerPocketCount());
}

void ButiEngine::Enemy::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Enemy::Clone()
{
	return ObjectFactory::Create<Enemy>();
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::Enemy::GetNearFreePocket(const Vector3& arg_pos, float arg_border)
{
	auto end = m_vec_pockets.end();
	for (auto itr = m_vec_pockets.begin(); itr != end; ++itr)
	{
		if (!(*itr).lock()->GetGameComponent<Pocket>()->ExistWorker())
		{
			float borderSqr = arg_border * arg_border;
			float distanceSqr = (arg_pos - (*itr).lock()->transform->GetWorldPosition()).GetLengthSqr();

			if (distanceSqr <= borderSqr)
			{
				return (*itr);
			}
		}
	}
	return Value_weak_ptr<GameObject>();
}

void ButiEngine::Enemy::CreatePocket(const std::uint8_t arg_pocketCount)
{
	RemoveAllPocket();
	m_pocketCount = arg_pocketCount;

	if (m_pocketCount == 0) { return; }

	//transform取得用worker
	//auto tmpWorker = GetManager().lock()->AddObjectFromCereal("Target");

	//自身の周りに等間隔でポケットを作成する
	float radius = gameObject.lock()->transform->GetLocalScale().x * 0.5f;
	float workerRadius = /*tmpWorker.lock()->transform->GetLocalScale().x*/1.0f * 0.5f;

	auto pocketCenter = gameObject.lock()->transform->Clone();
	auto pocketTransform = ObjectFactory::Create<Transform>();
	pocketTransform->SetBaseTransform(pocketCenter);
	pocketTransform->SetLocalPosition(Vector3(0.0f, 0.0f, (radius + workerRadius) / gameObject.lock()->transform->GetLocalScale().x));

	float rollAngle = 360.0f / m_pocketCount;

	for (std::uint8_t i = 0; i < m_pocketCount; i++)
	{
		auto pocket = GetManager().lock()->AddObjectFromCereal("Pocket", ObjectFactory::Create<Transform>());
		pocket.lock()->transform->SetBaseTransform(gameObject.lock()->transform);

		Vector3 pos = pocketTransform->GetWorldPosition();
		pocket.lock()->transform->SetWorldPosition(pos);
		pocket.lock()->transform->SetLocalScale(0.05f);

		auto pocketComponent = pocket.lock()->GetGameComponent<Pocket>();
		pocketComponent->SetEnemy(gameObject);

		m_vec_pockets.push_back(pocket);

		pocketCenter->RollLocalRotationY_Degrees(rollAngle);
	}

	//tmpWorker.lock()->SetIsRemove(true);
}

std::uint8_t ButiEngine::Enemy::GetExistWorkerPocketCount()
{
	std::uint8_t existWorkerPocketCount = 0;

	auto end = m_vec_pockets.end();
	for (auto itr = m_vec_pockets.begin(); itr != end; ++itr)
	{
		if ((*itr).lock()->GetGameComponent<Pocket>()->ExistWorker())
		{
			existWorkerPocketCount++;
		}
	}

	return existWorkerPocketCount;
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
