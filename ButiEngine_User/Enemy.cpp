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
#include "Enemy_Boss.h"
#include "SeparateDrawObject.h"
#include "VibrationEffectComponent.h"
#include "ShakeComponent.h"
#include "EnemyScaleAnimationComponent.h"
#include "ParticleGenerater.h"
#include "SpriteParticleGenerator.h"
#include "CameraShakeComponent.h"
#include "KnockBack.h"

float ButiEngine::Enemy::m_vibrationDecrease = 0.1f;
bool ButiEngine::Enemy::m_test_isExplosion = false;
float ButiEngine::Enemy::m_playerVibrationCoefficient = 3.0f;

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

	if (m_isNearPlayer || m_isHitShockWave)
	{
		CreateAttackFlashEffect();
	}
	else
	{
		m_vlp_attackFlashTimer->Stop();
		m_vlp_attackFlashTimer->Reset();
	}
	VibrationStickWoker();
	ShakeDrawObject();
	ScaleAnimation();
}

void ButiEngine::Enemy::OnSet()
{
	auto collisionEnterLambda = std::function<void(Value_weak_ptr<GameObject>&)>([this](Value_weak_ptr<GameObject>& arg_vwp_other)->void
		{
			if (arg_vwp_other.lock()->GetIsRemove()) { return; }
			if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("Enemy")))
			{
				OnCollisionEnemy(arg_vwp_other);
			}
		});

	auto collisionStayLambda = std::function<void(Value_weak_ptr<GameObject>&)>([this](Value_weak_ptr<GameObject>& arg_vwp_other)->void
		{
			if (arg_vwp_other.lock()->GetIsRemove()) { return; }
			if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("Sensor")))
			{
				m_isNearPlayer = true;
			}
			else if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("ShockWave")))
			{
				m_isHitShockWave = true;
			}
		});

	auto collisionLeaveLambda = std::function<void(Value_weak_ptr<GameObject>&)>([this](Value_weak_ptr<GameObject>& arg_vwp_other)->void
		{
			if (arg_vwp_other.lock()->GetIsRemove()) { return; }
			if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("Sensor")))
			{
				m_isNearPlayer = false;
			}
			else if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("ShockWave")))
			{
				m_isHitShockWave = false;
			}
		});

	gameObject.lock()->AddCollisionEnterReaction(collisionEnterLambda);
	gameObject.lock()->AddCollisionStayReaction(collisionStayLambda);
	gameObject.lock()->AddCollisionLeaveReaction(collisionLeaveLambda);


	m_vwp_player = GetManager().lock()->GetGameObject(GameObjectTag("Player"));
	m_vlp_playerComponent = m_vwp_player.lock()->GetGameComponent<Player>();
	m_vwp_waveManager = GetManager().lock()->GetGameObject("WaveManager").lock()->GetGameComponent<WaveManager>();

	m_defaultScale = gameObject.lock()->transform->GetLocalScale();

	m_isNearPlayer = false;
	m_isHitShockWave = false;
	m_stickWorkerCount = 0;
	m_vibration = 0.0f;
	m_vibrationIncrease = 0.0f;
	m_vibrationCapacity = 100.0f;
	m_vibrationResistance = 10.0f;
	m_weight = 100.0f;
	m_isCapaOver = false;

	m_explosionScale = 1.0f;
}

void ButiEngine::Enemy::OnRemove()
{
}

void ButiEngine::Enemy::OnShowUI()
{
	GUI::BulletText("Decrease");
	GUI::DragFloat("##decrease", &m_vibrationDecrease, 1.0f, 0.0f, 100.0f);
	GUI::BulletText("Capacity");
	GUI::DragFloat("##capacity", &m_vibrationCapacity, 1.0f, 0.0f, 1000.0f);
	GUI::BulletText("Resistance");
	GUI::DragFloat("##resistance", &m_vibrationResistance, 1.0f, 0.0f, 100.0f);
	GUI::BulletText("Coefficient");
	GUI::DragFloat("##coefficient", &m_playerVibrationCoefficient, 0.1f, 0.0f, 10.0f);
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
	m_vwp_particleGenerater = GetManager().lock()->GetGameObject("PolygonParticleController").lock()->GetGameComponent<ParticleGenerater>();
	m_vwp_spriteParticleGenerater = GetManager().lock()->GetGameObject("SpriteAnimationParticleController").lock()->GetGameComponent<SpriteParticleGenerator>();

	m_vlp_attackFlashTimer = ObjectFactory::Create<RelativeTimer>(6);
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
	return m_stickWorkerCount > 0 || m_isNearPlayer || m_isHitShockWave;
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
	AddDeadCount();
	StopVibrationEffect();

	m_vlp_playerComponent->SetIsIncrease(false);

	auto boss = gameObject.lock()->GetGameComponent<Enemy_Boss>();
	if (boss)
	{
		gameObject.lock()->SetIsRemove(true);
		return;
	}

	auto transform = gameObject.lock()->transform;
	auto deadEffect = GetManager().lock()->AddObjectFromCereal("SplashEffect");
	deadEffect.lock()->transform->SetLocalPosition(transform->GetLocalPosition());
	deadEffect.lock()->transform->SetLocalScale(m_defaultScale * 2.0f);

	gameObject.lock()->SetIsRemove(true);

	//GetManager().lock()->GetGameObject("Particle")

	//死んだら画面揺らす
	GetManager().lock()->GetGameObject("Camera").lock()->GetGameComponent<CameraShakeComponent>()->ShakeStart(2, 4);
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

void ButiEngine::Enemy::RemoveAllPocket()
{
	auto end = m_vec_pockets.end();
	for (auto itr = m_vec_pockets.begin(); itr != end; ++itr)
	{
		(*itr).lock()->GetGameComponent<Pocket>()->Dead();
	}
	m_vec_pockets.clear();
}

void ButiEngine::Enemy::IncreaseVibration()
{
	if (m_isCapaOver) { return; }

	CalculateVibrationIncrease();
	m_vibration += m_vibrationIncrease * GameDevice::WorldSpeed;

	//振動量が上限を超えたら死ぬ
	if (m_vibration > m_vibrationCapacity)
	{
		//ボスじゃなかったら即死
		if (!gameObject.lock()->HasGameObjectTag(GameObjectTag("Boss")))
		{
			Dead();
		}
		m_isCapaOver = true;
	}
}

void ButiEngine::Enemy::DecreaseVibration()
{
	if (m_isCapaOver) { return; }

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

void ButiEngine::Enemy::CreateAttackFlashEffect()
{
	m_vlp_attackFlashTimer->Start();

	Vector3 dir = (m_vwp_player.lock()->transform->GetLocalPosition() - gameObject.lock()->transform->GetLocalPosition()).GetNormalize();
	float radius = gameObject.lock()->transform->GetLocalScale().x * 0.5f;
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition() + dir * radius;

	float playerVibrationRate = m_vlp_playerComponent->GetVibrationRate();
	float size = MathHelper::Lerp(1.0f, 6.0f, playerVibrationRate) * 10.0f;

	std::uint8_t spawnIntervalFrame = MathHelper::Lerp(4, 1, playerVibrationRate);
	m_vlp_attackFlashTimer->ChangeCountFrame(spawnIntervalFrame);

	if (m_vlp_attackFlashTimer->Update())
	{
		m_vwp_spriteParticleGenerater.lock()->AttackFlashParticles(pos, 1.0f, size, Vector4(1.0f, 0.745, 0.0f, 1.0f));
	}
}


void ButiEngine::Enemy::CalculateVibrationIncrease()
{
	float playerVibrationForce = m_vlp_playerComponent->GetVibrationForce();
	float workerVibrationForce = Worker::GetVibrationForce();

	m_vibrationIncrease = 0.0f;

	if (m_isNearPlayer || m_isHitShockWave)
	{
		float playerVibration = m_vlp_playerComponent->GetVibration();
		m_vibrationIncrease += playerVibrationForce * (1 + playerVibration * m_playerVibrationCoefficient);
	}

	m_vibrationIncrease += workerVibrationForce * m_stickWorkerCount - m_vibrationResistance;
	m_vibrationIncrease = max(m_vibrationIncrease, 0.5f);
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

void ButiEngine::Enemy::AddDeadCount()
{
	//ウェーブマネージャーの討伐数カウント関数を呼ぶ
	m_vwp_waveManager.lock()->AddEnemyDeadCount();
}

void ButiEngine::Enemy::StopVibrationEffect()
{
	if (m_vwp_vibrationEffect.lock() != nullptr)
	{
		m_vwp_vibrationEffect.lock()->SetIsRemove(true);
		m_vwp_vibrationEffect = Value_weak_ptr<GameObject>();
	}
}

void ButiEngine::Enemy::OnCollisionEnemy(Value_weak_ptr<GameObject> arg_vwp_other)
{
	//重さが相手以下だったらノックバックされる
	auto vlp_enemyComponent = arg_vwp_other.lock()->GetGameComponent<Enemy>();
	float otherWeight = vlp_enemyComponent->GetWeight();

	if (m_weight <= otherWeight)
	{
		//ノックバック中だったら止める
		auto vlp_knockBack = gameObject.lock()->GetGameComponent<KnockBack>();
		if (vlp_knockBack)
		{
			vlp_knockBack->SetIsRemove(true);
		}

		Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
		Vector3 otherPos = arg_vwp_other.lock()->transform->GetLocalPosition();
		Vector3 dir = (pos - otherPos).GetNormalize();

		gameObject.lock()->AddGameComponent<KnockBack>(dir, 0.5f, false, 30);

		auto stalker = gameObject.lock()->GetGameComponent<Enemy_Stalker>();
		if (stalker)
		{
			stalker->ResetVelocity();
		}
	}
}
