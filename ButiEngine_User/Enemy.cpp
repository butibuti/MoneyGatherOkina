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
#include "Crystal.h"
#include "GameSettings.h"
#include "SoundPlayerComponent.h"
#include"EnemySpawnPointComponent.h"
#include "OutsideCrystal.h"
#include "Stick.h"
#include "PauseManagerComponent.h"

float ButiEngine::Enemy::m_vibrationDecrease = 0.1f;
float ButiEngine::Enemy::m_playerVibrationCoefficient = 3.0f;

void ButiEngine::Enemy::OnUpdate()
{
	if (GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_Y))
	{
		RuptureStickWorker();
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
		MobDamegeSE();
	}
	else
	{
		DecreaseVibration();
	}

	if (m_isHitShockWave)
	{
		AttackFlashUpdate();
	}
	else
	{
		m_vlp_attackFlashTimer->Stop();
		m_vlp_attackFlashTimer->Reset();
	}
	VibrationStickWoker();
	ShakeDrawObject();
	ScaleAnimation();

	if (m_vwp_pauseManagerComponent.lock()->IsPause())
	{
		StopMobDamageSE();
	}
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
			else if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("Sensor")))
			{
				//m_isNearPlayer = true;
				//m_vlp_attackFlashTimer->Start();
				//CreateAttackFlashEffect();
			}
			else if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("ShockWave")))
			{
				m_isHitShockWave = true;
				m_vlp_attackFlashTimer->Start();
				CreateAttackFlashEffect();
			}
		});

	auto collisionLeaveLambda = std::function<void(Value_weak_ptr<GameObject>&)>([this](Value_weak_ptr<GameObject>& arg_vwp_other)->void
		{
			if (arg_vwp_other.lock()->GetIsRemove()) { return; }
			if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("Sensor")))
			{
				//m_isNearPlayer = false;
			}
			else if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("ShockWave")))
			{
				m_isHitShockWave = false;
			}
		});

	gameObject.lock()->AddCollisionEnterReaction(collisionEnterLambda);
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
	m_isMobDamageSE = false;
	m_knockBackForce = 0.0f;
	m_knockBackFrame = 30;
}

void ButiEngine::Enemy::OnRemove()
{
}

void ButiEngine::Enemy::OnShowUI()
{
	GUI::BulletText(u8"振動値の減少量");
	GUI::DragFloat("##decrease", &m_vibrationDecrease, 1.0f, 0.0f, 100.0f);

	GUI::BulletText(u8"振動値上昇量の計算で使う係数");
	GUI::DragFloat("##coefficient", &m_playerVibrationCoefficient, 0.1f, 0.0f, 10.0f);

	GUI::BulletText(u8"振動値:%f / %f", m_vibration, m_vibrationCapacity);

	GUI::BulletText(u8"ついているモブハチの数:%d", GetStickWorkerCount());
}

void ButiEngine::Enemy::Start()
{
	m_vwp_particleGenerater = GetManager().lock()->GetGameObject("PolygonParticleController").lock()->GetGameComponent<ParticleGenerater>();
	m_vwp_spriteParticleGenerater = GetManager().lock()->GetGameObject("SpriteAnimationParticleController").lock()->GetGameComponent<SpriteParticleGenerator>();
	m_vwp_soundPlayerComponent = GetManager().lock()->GetGameObject("SoundPlayer").lock()->GetGameComponent<SoundPlayerComponent>();
	m_vwp_pauseManagerComponent = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManagerComponent>();

	m_vlp_attackFlashTimer = ObjectFactory::Create<RelativeTimer>(6);

	m_gameObjectName = gameObject.lock()->GetGameObjectName();
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
	return m_stickWorkerCount > 0 || m_isHitShockWave;
}

void ButiEngine::Enemy::Dead()
{
	auto position = gameObject.lock()->transform->GetWorldPosition();
	auto fly = gameObject.lock()->GetGameComponent<Enemy_Fly>();
	if (fly)
	{
		fly->Dead();
		m_vwp_particleGenerater.lock()->ExplosionPolygonParticles(position, false);
		m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/Defeat_Enemy_Small.wav"));
	}
	auto kiba = gameObject.lock()->GetGameComponent<Enemy_Kiba>();
	if (kiba)
	{
		kiba->Dead();
		m_vwp_particleGenerater.lock()->ExplosionPolygonParticles(position, true);
		m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/Defeat_Enemy_Big.wav"));
	}
	auto stalker = gameObject.lock()->GetGameComponent<Enemy_Stalker>();
	if (stalker)
	{
		stalker->Dead();
		m_vwp_particleGenerater.lock()->ExplosionPolygonParticles(position, false);
		m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/Defeat_Enemy_Small.wav"));
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
		m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/Defeat_Enemy_Big.wav"));
	}

	auto boss = gameObject.lock()->GetGameComponent<Enemy_Boss>();
	if (boss)
	{
		gameObject.lock()->SetIsRemove(true);
		return;
	}
	auto crystal = gameObject.lock()->GetGameComponent<Crystal>();
	if (crystal)
	{
		crystal->Dead();
		m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/Defeat_Enemy_Crystal.wav"));
		return;
	}

	auto transform = gameObject.lock()->transform;
	auto deadEffect = GetManager().lock()->AddObjectFromCereal("SplashEffect");
	deadEffect.lock()->transform->SetLocalPosition(transform->GetLocalPosition());
	deadEffect.lock()->transform->SetLocalScale(m_defaultScale * 2.0f);


	StopMobDamageSE();

	
	if (m_vwp_appearnceEffect.lock()) {
		m_vwp_appearnceEffect.lock()->GetGameComponent< EnemySpawnPointComponent>()->SetEnemyObject(Value_weak_ptr<GameObject>());
	}
	//死んだら画面揺らす
	GetManager().lock()->GetGameObject("Camera").lock()->GetGameComponent<CameraShakeComponent>()->ShakeStart(2, 4);

	AddPoint();

	auto outsideCrystal = gameObject.lock()->GetGameComponent<OutsideCrystal>();
	if (outsideCrystal)
	{
		outsideCrystal->Disappear();
		RuptureStickWorker();
		return;
	}

	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->Dead();

	RemoveAllPocket();
	StopVibrationEffect();

	gameObject.lock()->SetIsRemove(true);
}

void ButiEngine::Enemy::CreatePocket(const std::uint8_t arg_pocketCount, const float arg_radius)
{
	RemoveAllPocket();

	if (arg_pocketCount == 0) { return; }

	auto pocketCenter = gameObject.lock()->transform->Clone();
	auto pocketTransform = ObjectFactory::Create<Transform>();
	pocketTransform->SetBaseTransform(pocketCenter);
	pocketTransform->SetLocalPosition(Vector3(0.0f, 0.0f, arg_radius / gameObject.lock()->transform->GetLocalScale().x));

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

void ButiEngine::Enemy::SetAppearanceEffect(Value_weak_ptr<GameObject> arg_vwp_appearnceEffect)
{
	m_vwp_appearnceEffect = arg_vwp_appearnceEffect;
	m_vwp_appearnceEffect = arg_vwp_appearnceEffect;
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::Enemy::GconstetAppearanceEffect()
{
	return m_vwp_appearnceEffect;
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
				workerComponent->SetIsVibrate(IsVibrate());
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

void ButiEngine::Enemy::AttackFlashUpdate()
{
	if (m_vlp_attackFlashTimer->Update())
	{
		CreateAttackFlashEffect();
		auto soundTag = "Sound/Attack_OneShot_" + std::to_string(ButiRandom::GetInt(0, 3)) + ".wav";
		m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag(soundTag));
	}
}

void ButiEngine::Enemy::CreateAttackFlashEffect()
{
	Vector3 dir = (m_vwp_player.lock()->transform->GetLocalPosition() - gameObject.lock()->transform->GetLocalPosition()).GetNormalize();
	float radius = gameObject.lock()->transform->GetLocalScale().x * 0.5f;
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition() + dir * radius;

	float playerVibrationRate = m_vlp_playerComponent->GetVibrationRate();
	float size = MathHelper::Lerp(6.0f, 9.0f, playerVibrationRate) * 10.0f;

	Color color = GameSettings::PLAYER_COLOR;
	if (m_vlp_playerComponent->IsBomb())
	{
		color = GameSettings::SOUL_COLOR;
	}
	else if (m_vlp_playerComponent->IsOverheat())
	{
		color = GameSettings::ATTACK_COLOR;
	}

	m_vwp_spriteParticleGenerater.lock()->AttackFlashParticles(pos, 1.0f, size, color);

	std::uint8_t spawnIntervalFrame = MathHelper::Lerp(6, 1, playerVibrationRate);
	m_vlp_attackFlashTimer->ChangeCountFrame(spawnIntervalFrame);

	m_vlp_attackFlashTimer->Reset();
}


void ButiEngine::Enemy::CalculateVibrationIncrease()
{
	float playerVibrationForce = m_vlp_playerComponent->GetVibrationForce();
	float workerVibrationForce = Worker::GetVibrationForce();

	m_vibrationIncrease = 0.0f;

	if (m_isNearPlayer || m_isHitShockWave)
	{
		m_vibrationIncrease += m_vlp_playerComponent->GetVibrationForce();
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

void ButiEngine::Enemy::AddPoint()
{
	//ウェーブマネージャーの討伐数カウント関数を呼ぶ
	m_vwp_waveManager.lock()->AddPoint(m_progressPoint);
}

void ButiEngine::Enemy::StopVibrationEffect()
{
	if (m_vwp_vibrationEffect.lock() != nullptr)
	{
		m_vwp_vibrationEffect.lock()->SetIsRemove(true);
		m_vwp_vibrationEffect = Value_weak_ptr<GameObject>();
	}
}

void ButiEngine::Enemy::MobDamegeSE()
{
	//モブがくっついてなければそもそも鳴らさない
	if (m_stickWorkerCount <= 0) 
	{
		StopMobDamageSE();
		return;
	}


	//if (!m_isMobDamageSE)
	//{
	//	//再生
	//	m_isMobDamageSE = true;
	//	m_vwp_soundPlayerComponent.lock()->SetLoopIndex(m_gameObjectName); //ループ中としてインデックスを追加
	//	auto indexNum = m_vwp_soundPlayerComponent.lock()->GetLoopIndex(m_gameObjectName);
	//	m_vwp_soundPlayerComponent.lock()->PlayControllableSE(SoundTag("Sound/Attack_Loop.wav"), indexNum, 1, true);
	//}
}

void ButiEngine::Enemy::StopMobDamageSE()
{
	if (m_isMobDamageSE)
	{
		//再生中なら止める
		m_isMobDamageSE = false;
		auto indexNum = m_vwp_soundPlayerComponent.lock()->GetLoopIndex(m_gameObjectName);
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->DestroyControllableSE(indexNum);
		m_vwp_soundPlayerComponent.lock()->DestroyLoopIndex(m_gameObjectName); //ループ中のインデックスを削除
	}
}

void ButiEngine::Enemy::RuptureStickWorker()
{
	auto end = m_vec_pockets.end();
	for (auto itr = m_vec_pockets.begin(); itr != end; ++itr)
	{
		(*itr).lock()->GetGameComponent<Pocket>()->ReleaseWorker();
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

		gameObject.lock()->AddGameComponent<KnockBack>(dir, m_knockBackForce, false, m_knockBackFrame);

		auto stalker = gameObject.lock()->GetGameComponent<Enemy_Stalker>();
		if (stalker)
		{
			stalker->ResetVelocity();
		}
	}
}
