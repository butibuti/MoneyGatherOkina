#include "stdafx_u.h"
#include "Enemy.h"
#include "Pocket.h"
#include "Player.h"
#include "Worker.h"

float ButiEngine::Enemy::m_vibrationDecrease = 0.1f;

void ButiEngine::Enemy::OnUpdate()
{
	if (GameDevice::GetInput()->TriggerKey(Keys::L))
	{
		gameObject.lock()->SetIsRemove(true);
		return;
	}
	//プレイヤーが近くにいたら振動量上昇、いなかったら減少
	float nearBorderSqr = m_nearBorder * m_nearBorder;
	float distanceSqr = (gameObject.lock()->transform->GetLocalPosition() - m_vwp_player.lock()->transform->GetLocalPosition()).GetLengthSqr();
	if (distanceSqr <= nearBorderSqr)
	{
		IncreaseVibration();
	}
	else
	{
		DecreaseVibration();
	}
}

void ButiEngine::Enemy::OnSet()
{
	m_testPocketCount = 0;
	//gameObject.lock()->AddCollisionStayReaction(std::function<void(ButiBullet::ContactData&)>([](ButiBullet::ContactData& arg_other)->void 
	//	{
	//		int a = 0;
	//	}));
}

void ButiEngine::Enemy::OnRemove()
{
	RemoveAllPocket();
}

void ButiEngine::Enemy::OnShowUI()
{
	GUI::BulletText("NearBorder");
	GUI::DragFloat("##nearBorder", &m_nearBorder, 1.0f, 0.0f, 10.0f);
	GUI::BulletText("Decrease");
	GUI::DragFloat("##decrease", &m_vibrationDecrease, 1.0f, 0.0f, 100.0f);
	GUI::BulletText("Capacity");
	GUI::DragFloat("##capacity", &m_vibrationCapacity, 1.0f, 0.0f, 1000.0f);
	GUI::BulletText("Resistance");
	GUI::DragFloat("##resistance", &m_vibrationResistance, 1.0f, 0.0f, 100.0f);
	GUI::BulletText("Vibration:%f / %f", m_vibration, m_vibrationCapacity);
	GUI::BulletText("PocketCount");
	GUI::DragInt("##p", m_testPocketCount, 1.0f, 0, 20);
	if (GUI::Button("Create"))
	{
		CreatePocket(m_testPocketCount);
	}
	GUI::BulletText("StickWorkerCount:%d", GetStickWorkerCount());
}

void ButiEngine::Enemy::Start()
{
	m_vwp_player = GetManager().lock()->GetGameObject("Player");
	m_vlp_playerComponent = m_vwp_player.lock()->GetGameComponent<Player>();

	m_nearBorder = 3.0f;
	m_vibration = 0.0f;
	m_vibrationIncrease = 0.0f;
	m_vibrationCapacity = 100.0f;
	m_vibrationResistance = 10.0f;
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
		if (!(*itr).lock()->GetGameComponent<Pocket>()->ExistStickWorker())
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

void ButiEngine::Enemy::IncreaseVibration()
{
	CalcVibrationIncrease();
	m_vibration += m_vibrationIncrease;

	//振動量が上限を超えたら死ぬ
	if (m_vibration > m_vibrationCapacity)
	{
		gameObject.lock()->SetIsRemove(true);
	}
}

void ButiEngine::Enemy::DecreaseVibration()
{
	m_vibration -= m_vibrationDecrease;
	m_vibration = max(m_vibration, 0.0f);
}

void ButiEngine::Enemy::CalcVibrationIncrease()
{
	float playerVibrationForce = m_vlp_playerComponent->GetVibrationForce();
	float workerVibrationForce = Worker::GetVibrationForce();
	float stickWorkerCount = GetStickWorkerCount();

	m_vibrationIncrease = playerVibrationForce + workerVibrationForce * stickWorkerCount - m_vibrationResistance;
	m_vibrationIncrease = max(m_vibrationIncrease, 0.0f);
}

std::uint8_t ButiEngine::Enemy::GetStickWorkerCount()
{
	std::uint8_t stickWorkerCount = 0;

	auto end = m_vec_pockets.end();
	for (auto itr = m_vec_pockets.begin(); itr != end; ++itr)
	{
		if ((*itr).lock()->GetGameComponent<Pocket>()->ExistStickWorker())
		{
			stickWorkerCount++;
		}
	}

	return stickWorkerCount;
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
