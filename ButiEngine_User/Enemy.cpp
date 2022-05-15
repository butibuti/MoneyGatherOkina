#include "stdafx_u.h"
#include "Enemy.h"
#include "Pocket.h"
#include "Player.h"
#include "Worker.h"
#include "WaveManager.h"
#include "InputManager.h"
#include "PlayerSensor.h"
#include "Enemy_Fly.h"
#include "Enemy_Kiba.h"
#include "Enemy_Stalker.h"
#include "Enemy_Tutorial.h"
#include "Enemy_Volcano.h"
#include "SeparateDrawObject.h"
#include "VibrationEffectComponent.h"
#include "ShakeComponent.h"
#include "EnemyScaleAnimationComponent.h"
#include "ParticleGenerater.h"
#include "CameraShakeComponent.h"

float ButiEngine::Enemy::m_vibrationDecrease = 0.1f;
bool ButiEngine::Enemy::m_test_isExplosion = false;

void ButiEngine::Enemy::OnUpdate()
{
	if (GameDevice::GetInput()->TriggerKey(Keys::B))
	{
		m_test_isExplosion = !m_test_isExplosion;
	}
	//Playerが近いか衝撃波が当たっていたら振動する
	if (m_vibration > 0)
	{
		//if (m_vwp_vibrationEffect.lock() == nullptr)
		//{
		//	auto transform = gameObject.lock()->transform;
		//	m_vwp_vibrationEffect = GetManager().lock()->AddObjectFromCereal("VibrationEffect");
		//	m_vwp_vibrationEffect.lock()->transform->SetLocalPosition(transform->GetLocalPosition());
		//	m_vwp_vibrationEffect.lock()->transform->SetLocalScale(m_defaultScale * 1.5f);

		//	m_vwp_vibrationEffectComponent = m_vwp_vibrationEffect.lock()->GetGameComponent<VibrationEffectComponent>();
		//}
		//else
		//{
		//	auto transform = gameObject.lock()->transform;
		//	float vibrationPower = m_vibration / m_vibrationCapacity;
		//	m_vwp_vibrationEffectComponent.lock()->SetVibrationViolent(vibrationPower, false);
		//	m_vwp_vibrationEffectComponent.lock()->SetEffectPosition(transform->GetLocalPosition());
		//}
	}
	else
	{
		//StopVibrationEffect();
	}

	if (IsVibrate())
	{
		IncreaseVibration();	
	}
	else
	{
		DecreaseVibration();
	}
	VibrationStickWoker();
	ShakeDrawObject();
	ScaleAnimation();
}

void ButiEngine::Enemy::OnSet()
{
	auto collisionStayLambda = std::function<void(Value_weak_ptr<GameObject>&)>([this](Value_weak_ptr<GameObject>& arg_vwp_other)->void
		{
			if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("ShockWave")))
			{
				m_isHitShockWave = true;
			}
		});

	auto collisionLeaveLambda = std::function<void(Value_weak_ptr<GameObject>&)>([this](Value_weak_ptr<GameObject>& arg_vwp_other)->void
		{
			if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("ShockWave")))
			{
				m_isHitShockWave = false;
			}
		});

	gameObject.lock()->AddCollisionStayReaction(collisionStayLambda);
	gameObject.lock()->AddCollisionLeaveReaction(collisionLeaveLambda);


	m_vwp_player = GetManager().lock()->GetGameObject(GameObjectTag("Player"));
	m_vlp_playerComponent = m_vwp_player.lock()->GetGameComponent<Player>();
	m_vwp_waveManager = GetManager().lock()->GetGameObject("WaveManager").lock()->GetGameComponent<WaveManager>();

	m_defaultScale = gameObject.lock()->transform->GetLocalScale();

	m_vwp_particleGenerater = GetManager().lock()->GetGameObject("PolygonParticleController").lock()->GetGameComponent<ParticleGenerater>();

	m_isNearPlayer = false;
	m_isHitShockWave = false;
	m_vibration = 0.0f;
	m_vibrationIncrease = 0.0f;
	m_vibrationCapacity = 100.0f;
	m_vibrationResistance = 10.0f;

	m_explosionScale = 1.0f;
}

void ButiEngine::Enemy::OnRemove()
{
}

void ButiEngine::Enemy::OnShowUI()
{
	float nearBorder = m_vwp_playerSensor.lock()->transform->GetWorldScale().x * 0.5f;
	GUI::BulletText("NearBorder");
	if (GUI::DragFloat("##nearBorder", &nearBorder, 0.1f, 0.0f, 100.0f))
	{
		SetNearBorder(nearBorder);
	}
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

	GUI::BulletText("ExplosionScale");
	GUI::DragFloat("##exScale", &m_explosionScale, 0.1f, 0.0f, 100.0f);
}

void ButiEngine::Enemy::Start()
{
	m_vwp_playerSensor = GetManager().lock()->AddObjectFromCereal("PlayerSensor");
	m_vwp_playerSensor.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);
	m_vwp_playerSensor.lock()->GetGameComponent<PlayerSensor>()->SetParentEnemy(gameObject);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Enemy::Clone()
{
	return ObjectFactory::Create<Enemy>();
}

void ButiEngine::Enemy::SetNearBorder(const float arg_nearBorder)
{
	Vector3 scale = gameObject.lock()->transform->GetLocalScale();
	Vector3 sensorScale = Vector3(arg_nearBorder * 2) / scale;
	m_vwp_playerSensor.lock()->transform->SetLocalScale(sensorScale);
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

std::vector<ButiEngine::Value_weak_ptr<ButiEngine::GameObject>> ButiEngine::Enemy::GetStickWorkers()
{
	std::vector<Value_weak_ptr<GameObject>> vec_stickWorkers;
	auto end = m_vec_pockets.end();
	for (auto itr = m_vec_pockets.begin(); itr != end; ++itr)
	{
		auto worker = (*itr).lock()->GetGameComponent<Pocket>()->GetWorker();
		if (worker.lock() != nullptr)
		{
			vec_stickWorkers.push_back(worker);
		}
	}
	return vec_stickWorkers;
}

bool ButiEngine::Enemy::IsVibrate()
{
	return m_isNearPlayer || m_isHitShockWave;
}

void ButiEngine::Enemy::Dead()
{
	auto position = gameObject.lock()->transform->GetWorldPosition();
	auto fly = gameObject.lock()->GetGameComponent<Enemy_Fly>();
	if (fly)
	{
		fly->Dead();
		m_vwp_particleGenerater.lock()->ExplosionPolygonParticles(position, false);
	}
	auto kiba = gameObject.lock()->GetGameComponent<Enemy_Kiba>();
	if (kiba)
	{
		kiba->Dead();
		m_vwp_particleGenerater.lock()->ExplosionPolygonParticles(position, true);
	}
	auto stalker = gameObject.lock()->GetGameComponent<Enemy_Stalker>();
	if (stalker)
	{
		stalker->Dead();
		m_vwp_particleGenerater.lock()->ExplosionPolygonParticles(position, false);
	}
	auto tutorial = gameObject.lock()->GetGameComponent<Enemy_Tutorial>();
	if (tutorial)
	{
		tutorial->Dead();
		m_vwp_particleGenerater.lock()->ExplosionPolygonParticles(position, false);
	}
	auto volcano = gameObject.lock()->GetGameComponent<Enemy_Volcano>();
	if (volcano)
	{
		volcano->Dead();
		m_vwp_particleGenerater.lock()->ExplosionPolygonParticles(position, true);
	}

	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->Dead();

	RemoveAllPocket();
	SubDeadCount();
	StopVibrationEffect();

	if (m_vwp_playerSensor.lock())
	{
		m_vwp_playerSensor.lock()->GetGameComponent<PlayerSensor>()->Dead();
	}

	auto transform = gameObject.lock()->transform;
	auto deadEffect = GetManager().lock()->AddObjectFromCereal("SplashEffect");
	deadEffect.lock()->transform->SetLocalPosition(transform->GetLocalPosition());
	deadEffect.lock()->transform->SetLocalScale(m_defaultScale * 2.0f);

	gameObject.lock()->SetIsRemove(true);

	//GetManager().lock()->GetGameObject("Particle")

	//死んだら画面揺らす
	GetManager().lock()->GetGameObject("Camera").lock()->GetGameComponent<CameraShakeComponent>()->ShakeStart(2, 30);
}

void ButiEngine::Enemy::Explosion()
{
	if (!m_test_isExplosion) { return; }
	auto transform = gameObject.lock()->transform->Clone();
	transform->SetLocalScale(m_explosionScale);
	GetManager().lock()->AddObjectFromCereal("Explosion", transform);
}

void ButiEngine::Enemy::CreatePocket(const std::uint8_t arg_pocketCount)
{
	RemoveAllPocket();

	if (arg_pocketCount == 0) { return; }

	//transform取得用worker
	//auto tmpWorker = GetManager().lock()->AddObjectFromCereal("Target");

	//自身の周りに等間隔でポケットを作成する
	float radius = gameObject.lock()->transform->GetLocalScale().x * 0.5f;
	float workerRadius = /*tmpWorker.lock()->transform->GetLocalScale().x*/0.5f * 0.5f;

	auto pocketCenter = gameObject.lock()->transform->Clone();
	auto pocketTransform = ObjectFactory::Create<Transform>();
	pocketTransform->SetBaseTransform(pocketCenter);
	pocketTransform->SetLocalPosition(Vector3(0.0f, 0.0f, (radius + workerRadius) / gameObject.lock()->transform->GetLocalScale().x));

	float rollAngle = 360.0f / arg_pocketCount;

	for (std::uint8_t i = 0; i < arg_pocketCount; i++)
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

void ButiEngine::Enemy::RemovePocket(const std::uint8_t arg_pocketNum)
{
	if (arg_pocketNum >= m_vec_pockets.size()) { return; }
	m_vec_pockets[arg_pocketNum].lock()->GetGameComponent<Pocket>()->Dead();
	m_vec_pockets.erase(m_vec_pockets.begin() + arg_pocketNum);
}

void ButiEngine::Enemy::IncreaseVibration()
{
	CalculateVibrationIncrease();
	m_vibration += m_vibrationIncrease * GameDevice::WorldSpeed;

	//振動量が上限を超えたら死ぬ
	if (m_vibration > m_vibrationCapacity)
	{
		Dead();
	}
}

void ButiEngine::Enemy::DecreaseVibration()
{
	m_vibration -= m_vibrationDecrease * GameDevice::WorldSpeed;
	m_vibration = max(m_vibration, 0.0f);
}

void ButiEngine::Enemy::VibrationStickWoker()
{
	auto vec_stickWorkers = GetStickWorkers();
	auto end = vec_stickWorkers.end();
	for (auto itr = vec_stickWorkers.begin(); itr != end; ++itr)
	{
		if ((*itr).lock() != nullptr)
		{
			auto workerComponent = (*itr).lock()->GetGameComponent<Worker>();
			if (workerComponent)
			{
				workerComponent->SetVibration(IsVibrate());
			}
		}
		
	}
}

void ButiEngine::Enemy::ShakeDrawObject()
{
	if (!m_vwp_shakeComponent.lock())
	{
		m_vwp_shakeComponent = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock()->GetGameComponent<ShakeComponent>();
		m_vwp_shakeComponent.lock()->ShakeStart();
		return;
	}

	float vibrationRate = m_vibration / m_vibrationCapacity;
	m_vwp_shakeComponent.lock()->SetShakePower(vibrationRate);
}

void ButiEngine::Enemy::ScaleAnimation()
{
	if (!m_vwp_scaleAnimationComponent.lock())
	{
		m_vwp_scaleAnimationComponent = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock()->GetGameComponent<EnemyScaleAnimationComponent>();
		return;
	}

	//ここでスケール割合値をセットしてあげる
	float lerpScale = m_vibration / m_vibrationCapacity;
	m_vwp_scaleAnimationComponent.lock()->SetScaleRate(lerpScale);
}


void ButiEngine::Enemy::CalculateVibrationIncrease()
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
		(*itr).lock()->GetGameComponent<Pocket>()->Dead();
	}
	m_vec_pockets.clear();
}

void ButiEngine::Enemy::SubDeadCount()
{
	//ウェーブマネージャーのカウント減少関数を呼ぶ
	m_vwp_waveManager.lock()->SubEnemyDeadCount();
}

void ButiEngine::Enemy::StopVibrationEffect()
{
	if (m_vwp_vibrationEffect.lock() != nullptr)
	{
		m_vwp_vibrationEffect.lock()->SetIsRemove(true);
		m_vwp_vibrationEffect = Value_weak_ptr<GameObject>();
	}
}
