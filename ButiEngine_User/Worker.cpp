#include "stdafx_u.h"
#include "Worker.h"
#include "Enemy.h"
#include "Pocket.h"
#include "Stick.h"
#include "SeparateDrawObject.h"
#include "Flocking.h"
#include "MoveRestriction.h"
#include "Player.h"
#include "WorkerSpawner.h"
#include "SphereExclusion.h"
#include "Enemy_Stalker.h"
#include "VibrationEffectComponent.h"
#include "ShakeComponent.h"
#include "BeeSoulComponent.h"
#include "ParticleGenerater.h"
#include "SpriteParticleGenerator.h"
#include "TiltMotion.h"
#include "FloatMotionComponent.h"
#include "KnockBack.h"
#include "SoundPlayerComponent.h"
#include "EnemyScaleAnimationComponent.h"
#include "GameSettings.h"
#include "SpawnEffect.h"
#include "WaveManager.h"
#include "WingAnimation.h"
#include "CameraShakeComponent.h"
#include "WorldSpeedManager.h"
#include "ShockWave.h"

float ButiEngine::Worker::m_nearBorder = 2.0f;
float ButiEngine::Worker::m_maxVibration = 130.0f;
float ButiEngine::Worker::m_minVibration = 20.0f;
float ButiEngine::Worker::m_vibrationIncrease = 0.21f;
float ButiEngine::Worker::m_vibrationDecrease = 0.19f;
float ButiEngine::Worker::m_maxScaleRate = 2.0f;
float ButiEngine::Worker::m_initVibrationForce = 0.1f;
float ButiEngine::Worker::m_maxVibrationMagnification = 3.0f;
float ButiEngine::Worker::m_spawnParticleSpeed = 0.5f;

void ButiEngine::Worker::OnUpdate()
{
	if (!m_vwp_waveManager.lock()->IsGameStart())
	{
		return;
	}

	//if (m_isNearPlayer)
	//{
	//	OnNearPlayer();
	//}

	if (m_vwp_flocking.lock() && m_vwp_flocking.lock()->IsActive() && m_vwp_flocking.lock()->GetMoveSpeed() > m_spawnParticleSpeed)
	{
		m_vwp_sphereParticleGenerator.lock()->AfterImageParticles(gameObject.lock()->transform->GetLocalPosition());
	}

	if (m_isTutorialVibrationObject && m_vibration < m_tutorialMaxVibration && m_vlp_vibrationResetTimer->Update())
	{
		m_vlp_vibrationResetTimer->Reset();
		m_vibration = m_tutorialMaxVibration;
	}

	if (m_isRupture)
	{
		OnRupture();
	}

	//if (m_isVibrate)
	//{

	//	//ShakeDrawObject();
	//}
	//else
	//{
	//	StopVibrationEffect();
	//	//StopShakeDrawObject();
	//}
	if (!m_vwp_vibrationEffect.lock())
	{
		auto transform = gameObject.lock()->transform;
		m_vwp_vibrationEffect = GetManager().lock()->AddObjectFromCereal("VibrationEffect_NoBloom");
		m_vwp_vibrationEffect.lock()->transform->SetLocalPosition(transform->GetWorldPosition());
		m_vwp_vibrationEffect.lock()->transform->SetLocalScale(0.0f);

		m_vwp_vibrationEffectComponent = m_vwp_vibrationEffect.lock()->GetGameComponent<VibrationEffectComponent>();
		m_vwp_vibrationEffectComponent.lock()->SetDefaultScale(m_defaultScale * 4.0f);
		auto meshDraw = m_vwp_vibrationEffect.lock()->GetGameComponent<MeshDrawComponent>();
		Vector4 color = GameSettings::WORKER_COLOR * 0.65f;
		color.w = 1.0f;
		meshDraw->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color = color;
	}
	else
	{
		auto transform = gameObject.lock()->transform;
		float vibrationRate = m_vibration / m_maxVibration;
		m_vwp_vibrationEffectComponent.lock()->SetVibrationViolent(vibrationRate, false);
		m_vwp_vibrationEffectComponent.lock()->SetEffectPosition(transform->GetWorldPosition());
		if (m_vwp_shockWave.lock())
		{
			m_vwp_shockWave.lock()->GetGameComponent<ShockWave>()->SetScale(vibrationRate * 2.5f);
		}
	}
	ShakeDrawObject();

	if (m_isAttack)
	{
		IncreaseVibration();
		CreateAttackFlashEffect();
	}
	else
	{
		DecreaseVibration();
		m_vlp_attackFlashTimer->Stop();
		m_vlp_attackFlashTimer->Reset();
	}

	ScaleAnimation();
}

void ButiEngine::Worker::OnSet()
{
	auto collisionLambda = std::function<void(Value_ptr<GameObject>&)>([this](Value_ptr<GameObject>& arg_vwp_other)->void
		{
			if (arg_vwp_other->transform->GetWorldScale() == Vector3Const::Zero) { return; }
			if (arg_vwp_other->GetIsRemove()) { return; }
			if (arg_vwp_other->HasGameObjectTag(GameObjectTag("Sensor")))
			{
				OnCollisionPlayer(arg_vwp_other);
			}
			//else if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("Stalker")))
			//{
			//	OnCollisionStalker(arg_vwp_other);
			//}
			else if (arg_vwp_other->HasGameObjectTag(GameObjectTag("Enemy")))
			{
				OnCollisionEnemy(arg_vwp_other);
			}
			//else if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("WorlerDamageArea")))
			//{
			//	OnCollisionDamageArea(arg_vwp_other);
			//}
			//else if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("DamageArea")))
			//{
			//	OnCollisionDamageArea(arg_vwp_other);
			//}
		});

	gameObject.lock()->AddCollisionEnterReaction(collisionLambda);
	gameObject.lock()->AddCollisionStayReaction(collisionLambda);
}

void ButiEngine::Worker::OnRemove()
{
}

void ButiEngine::Worker::OnShowUI()
{
	GUI::BulletText("NearBorder");
	GUI::DragFloat("##nearBorder", &m_nearBorder, 0.1f, 0.0f, 10.0f);

	GUI::Text(u8"?U???l:%f", m_vibration);

	GUI::BulletText(u8"?????U???l");
	GUI::DragFloat("##maxVibration", &m_maxVibration, 1.0f, 1.0f, 1000.0f);

	GUI::BulletText(u8"?????U???l");
	GUI::DragFloat("##minVibration", &m_minVibration, 1.0f, 0.0f, 1000.0f);

	GUI::BulletText(u8"?`???[?g???A?????????U???l");
	GUI::DragFloat("##tutorialMaxVibration", &m_tutorialMaxVibration, 1.0f, 0.0f, 1000.0f);

	GUI::BulletText(u8"?U????:%f", GetVibrationForce());

	GUI::BulletText(u8"?????U????");
	GUI::DragFloat("##vMinForce", &m_initVibrationForce, 1.0f, 0.0f, 100.0f);

	GUI::BulletText(u8"?U???????????{??");
	GUI::DragFloat("##vMaxForce", &m_maxVibrationMagnification, 1.0f, 0.0f, 100.0f);

	GUI::BulletText(u8"?U???l????????");
	GUI::DragFloat("##vIncrease", &m_vibrationIncrease, 0.001f, 0.0f, 1.0f);

	GUI::BulletText(u8"?U???l????????");
	GUI::DragFloat("##vDecrease", &m_vibrationDecrease, 0.001f, 0.0f, 1.0f);

	GUI::BulletText(u8"?p?[?e?B?N???o???n???????x");
	GUI::DragFloat("##spawnParticleSpeed", &m_spawnParticleSpeed, 0.01f, 0.0f, 10.0f);
}

void ButiEngine::Worker::Start()
{
	m_vwp_waveManager = GetManager().lock()->GetGameObject("WaveManager").lock()->GetGameComponent<WaveManager>();

	m_isTutorialVibrationObject = gameObject.lock()->HasGameObjectTag(GameObjectTag("VibrationObject"));
	m_vwp_soundPlayerComponent = GetManager().lock()->GetGameObject("SoundPlayer").lock()->GetGameComponent<SoundPlayerComponent>();

	if (m_isTutorialVibrationObject)
	{
		m_vlp_vibrationResetTimer = ObjectFactory::Create<RelativeTimer>(180);
		m_vlp_vibrationResetTimer->Start();

		m_vwp_shockWave = GetManager().lock()->AddObjectFromCereal("ShockWave_Worker");
		m_vwp_shockWave.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);
		m_vwp_shockWave.lock()->GetGameComponent<ShockWave>()->SetParent(gameObject);
	}
	else if (m_vwp_waveManager.lock()->IsTutorial())
	{
		auto spawnEffect = GetManager().lock()->AddObjectFromCereal("SpawnEffect");
		spawnEffect.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition());
		spawnEffect.lock()->GetGameComponent<SpawnEffect>()->SetColor(GameSettings::WORKER_COLOR);
		m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/Beam.wav"));
	}
	else
	{
		gameObject.lock()->transform->SetWorldPostionY(-1000.0f);
	}

	CreateDrawObject();

	gameObject.lock()->GetGameComponent<SphereExclusion>()->SetWeight(1.0f);

	SetLookAtParameter();
	SetVibrationParameter();

	m_defaultScale = gameObject.lock()->transform->GetLocalScale();
	m_isPredated = false;
	m_isAttack = false;

	m_vlp_player = GetManager().lock()->GetGameObject(GameObjectTag("Player")).lock()->GetGameComponent<Player>();

	m_vwp_flocking = gameObject.lock()->GetGameComponent<Flocking>();
	m_vwp_particleGenerater = GetManager().lock()->GetGameObject("BillBoardParticleController").lock()->GetGameComponent<ParticleGenerater>();
	m_vwp_spriteParticleGenerator = GetManager().lock()->GetGameObject("SpriteAnimationParticleController").lock()->GetGameComponent<SpriteParticleGenerator>();
	m_vwp_sphereParticleGenerator = GetManager().lock()->GetGameObject("SphereParticleController_NoBloom").lock()->GetGameComponent<SpriteParticleGenerator>();


	m_vlp_attackFlashTimer = ObjectFactory::Create<RelativeTimer>(6);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Worker::Clone()
{
	return ObjectFactory::Create<Worker>();
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::Worker::GetDrawObject()
{
	if (!m_vwp_tiltFloatObject.lock()) { return Value_weak_ptr<GameObject>(); }
	return m_vwp_tiltFloatObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject();
}

void ButiEngine::Worker::Spawn()
{
	gameObject.lock()->transform->SetWorldPostionY(0.0f);

	auto spawnEffect = GetManager().lock()->AddObjectFromCereal("SpawnEffect");
	spawnEffect.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition());
	spawnEffect.lock()->transform->SetLocalScale(2.0f);
	spawnEffect.lock()->GetGameComponent<SpawnEffect>()->SetColor(GameSettings::WORKER_COLOR);

	m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag("Sound/Beam.wav"));

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 front = gameObject.lock()->transform->GetFront();
	front.y = 0.0f;
	gameObject.lock()->transform->SetLookAtRotation(pos + front * 100.0f);
	m_vlp_lookAt->GetLookTarget()->SetLocalPosition(pos + front * 100.0f);

	auto transform = gameObject.lock()->transform;
	auto deadEffect = GetManager().lock()->AddObjectFromCereal("SplashEffect_NoBloom");
	deadEffect.lock()->transform->SetLocalPosition(transform->GetLocalPosition());
	auto randomRotate = (float)ButiRandom::GetInt(-180, 180);
	deadEffect.lock()->transform->SetLocalRotationZ_Degrees(randomRotate);
	deadEffect.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color = GameSettings::WORKER_COLOR;

	GetManager().lock()->GetGameObject("Camera").lock()->GetGameComponent<CameraShakeComponent>()->ShakeStart(0.5, 4);
}

void ButiEngine::Worker::Dead()
{
	//m_vwp_beeSoul = GetManager().lock()->AddObjectFromCereal("BeeSoul");
	//m_vwp_beeSoul.lock()->GetGameComponent<BeeSoulComponent>()->SetPosition(gameObject.lock()->transform->GetWorldPosition());

	auto stick = gameObject.lock()->GetGameComponent<Stick>();
	if (stick)
	{
		stick->Dead();
	}

	if (m_vwp_shockWave.lock())
	{
		m_vwp_shockWave.lock()->SetIsRemove(true);
	}

	if (m_vwp_vibrationEffect.lock())
	{
		m_vwp_vibrationEffect.lock()->SetIsRemove(true);
	}

	if (m_vwp_tiltFloatObject.lock())
	{
		m_vwp_tiltFloatObject.lock()->GetGameComponent<SeparateDrawObject>()->Dead();
		m_vwp_tiltFloatObject.lock()->SetIsRemove(true);
	}

	gameObject.lock()->SetIsRemove(true);
}

void ButiEngine::Worker::Rupture(const Vector3& arg_dir)
{
	if (m_isRupture) { return; }

	auto stick = gameObject.lock()->GetGameComponent<Stick>();
	if (stick)
	{
		stick->Dead();
	}

	auto tiltMotion = m_vwp_tiltFloatObject.lock()->GetGameComponent<TiltMotion>();
	if (tiltMotion)
	{
		tiltMotion->SetIsActive(false);
	}

	auto floatMotion = m_vwp_tiltFloatObject.lock()->GetGameComponent<FloatMotionComponent>();
	if (floatMotion)
	{
		floatMotion->SetIsActive(false);
	}

	m_isVibrate = false;
	StopVibrationEffect();

	std::int8_t frame = 120;

	Vector3 dir = arg_dir;
	dir.y = 4.0f;
	dir.Normalize();
	gameObject.lock()->AddGameComponent<KnockBack>(dir, 0.5f, true, frame);
	//m_vlp_ruptureTimer = ObjectFactory::Create<RelativeTimer>(frame + 60);
	//m_vlp_ruptureTimer->Start();

	m_isAttack = false;
	m_isRupture = true;
}

void ButiEngine::Worker::Predated(Value_weak_ptr<GameObject> arg_vwp_other)
{
	//???H??????????????????????
	if (m_isPredated) { return; }

	if (m_vwp_flocking.lock())
	{
		m_vwp_flocking.lock()->RemoveFlocking();
	}

	if (m_vlp_lookAt)
	{
		m_vlp_lookAt->SetIsActive(false);
	}

	auto collider = gameObject.lock()->GetGameComponent<Collision::ColliderComponent>();
	if (collider)
	{
		collider->CollisionStop();
	}

	auto tiltMotion = m_vwp_tiltFloatObject.lock()->GetGameComponent<TiltMotion>();
	if (tiltMotion)
	{
		tiltMotion->SetIsActive(false);
	}

	auto floatMotion = m_vwp_tiltFloatObject.lock()->GetGameComponent<FloatMotionComponent>();
	if (floatMotion)
	{
		floatMotion->SetIsActive(false);
	}

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 stalkerPos = arg_vwp_other.lock()->transform->GetLocalPosition();
	float radius = gameObject.lock()->transform->GetLocalScale().x * 0.5f;
	float stalkerRadius = arg_vwp_other.lock()->transform->GetLocalScale().x * 0.5f;
	Vector3 stalkerFront = arg_vwp_other.lock()->transform->GetFront();
	Vector3 newPos = stalkerPos + stalkerFront * (radius + stalkerRadius);

	gameObject.lock()->transform->SetLocalPosition(newPos);
	gameObject.lock()->transform->SetBaseTransform(arg_vwp_other.lock()->transform);

	m_isPredated = true;
}

void ButiEngine::Worker::CreateAttackFlashEffect()
{
	Vector3 pos = gameObject.lock()->transform->GetWorldPosition();
	Vector3 front = gameObject.lock()->transform->GetFront();
	float radius = gameObject.lock()->transform->GetWorldScale().x * 0.5f;
	pos += front * radius;

	if (m_vlp_attackFlashTimer->Update())
	{
		m_vwp_spriteParticleGenerator.lock()->AttackFlashParticles(pos, 0.5f, 20.0f, Vector4(0.35, 1.0f, 1.0f, 1.0f));
	}
}

void ButiEngine::Worker::OnCollisionPlayer(Value_weak_ptr<GameObject> arg_vwp_other)
{
	return;
	if (m_isNearPlayer) { return; }

	float playerVibrationRate = m_vlp_player->GetVibrationRate();
	if (playerVibrationRate >= 1.0f)
	{
		if (m_vwp_flocking.lock())
		{
			m_vwp_flocking.lock()->RemoveFlocking();
		}

		auto collider = gameObject.lock()->GetGameComponent<Collision::ColliderComponent>();
		if (collider)
		{
			collider->CollisionStop();
		}

		m_vlp_nearPlayerTimer = ObjectFactory::Create<RelativeTimer>(30);
		m_vlp_nearPlayerTimer->Start();
		m_isNearPlayer = true;
		m_isVibrate = true;
	}
}

void ButiEngine::Worker::OnCollisionStalker(Value_weak_ptr<GameObject> arg_vwp_other)
{
	//???H??????????????????????
	if (m_isPredated) { return; }

	auto stalker = arg_vwp_other.lock()->GetGameComponent<Enemy_Stalker>();
	if (!stalker) { return; }

	//???H????????????????
	bool isPrey = stalker->IsPrey();
	if (isPrey)
	{
		OnCollisionEnemy(arg_vwp_other);
		return; 
	}

	Predated(arg_vwp_other);
}

void ButiEngine::Worker::OnCollisionEnemy(Value_weak_ptr<GameObject> arg_vwp_enemy)
{
	//???H??????????????????????
	if (m_isPredated) { return; }
	//????????????????????????????????
	if (gameObject.lock()->GetGameComponent<Stick>()) { return; }

	//?G?????????????|?P?b?g????????????????????
	auto enemyComponent = arg_vwp_enemy.lock()->GetGameComponent<Enemy>();
	if (!enemyComponent) { return; }

	auto pocket = enemyComponent->GetNearFreePocket(gameObject.lock()->transform->GetLocalPosition(), m_nearBorder);

	if (pocket.lock())
	{
		gameObject.lock()->transform->SetLookAtRotation(arg_vwp_enemy.lock()->transform->GetLocalPosition());

		enemyComponent->AddStickWorkerCount();

		if (m_vwp_flocking.lock())
		{
			m_vwp_flocking.lock()->RemoveFlocking();
		}

		auto collider = gameObject.lock()->GetGameComponent<Collision::ColliderComponent>();
		if (collider)
		{
			collider->CollisionStop();
		}

		auto lookAt = gameObject.lock()->GetGameComponent<LookAtComponent>();
		if (lookAt)
		{
			lookAt->SetIsActive(false);
		}

		auto tiltMotion = m_vwp_tiltFloatObject.lock()->GetGameComponent<TiltMotion>();
		if (tiltMotion)
		{
			tiltMotion->SetIsActive(false);
		}

		m_isAttack = true;
		m_vlp_attackFlashTimer->Start();

		auto stickComponent = gameObject.lock()->AddGameComponent<Stick>();
		stickComponent->SetPocket(pocket);

		auto soundTag = "Sound/Attack_OneShot_" + std::to_string(ButiRandom::GetInt(0, 2)) + ".wav";
		m_vwp_soundPlayerComponent.lock()->PlaySE(SoundTag(soundTag));
	}
}

void ButiEngine::Worker::OnCollisionDamageArea(Value_weak_ptr<GameObject> arg_vwp_other)
{
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 damageAreaPos = arg_vwp_other.lock()->transform->GetWorldPosition();
	Vector3 dir = (pos - damageAreaPos).GetNormalize();
	Rupture(dir);
}

void ButiEngine::Worker::OnNearPlayer()
{
	if (m_isRupture) { return; }

	if (m_vlp_nearPlayerTimer->Update())
	{
		auto player = GetManager().lock()->GetGameObject(GameObjectTag("Player"));
		Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
		Vector3 playerPos = player.lock()->transform->GetLocalPosition();
		Vector3 dir = (pos - playerPos).GetNormalize();
		Rupture(dir);
	}
}

void ButiEngine::Worker::OnRupture()
{
	auto knockBack = gameObject.lock()->GetGameComponent<KnockBack>();
	if (/*m_vlp_ruptureTimer->Update()*/ !knockBack)
	{
		//m_vlp_ruptureTimer->Stop();
		//m_vlp_ruptureTimer->Reset();
		m_isRupture = false;
		ReturnFlock();
	}
}

void ButiEngine::Worker::ReturnFlock()
{
	if (m_vwp_flocking.lock())
	{
		m_vwp_flocking.lock()->AddFlocking();
	}

	auto collider = gameObject.lock()->GetGameComponent<Collision::ColliderComponent>();
	if (collider)
	{
		collider->CollisionStart();
	}

	auto lookAt = gameObject.lock()->GetGameComponent<LookAtComponent>();
	if (lookAt)
	{
		lookAt->SetIsActive(true);
		Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
		Vector3 front = gameObject.lock()->transform->GetFront();
		front.y = 0.0f;
		gameObject.lock()->transform->SetLookAtRotation(pos + front * 100.0f);
		lookAt->GetLookTarget()->SetLocalPosition(pos + front * 100.0f);
	}

	auto tiltMotion = m_vwp_tiltFloatObject.lock()->GetGameComponent<TiltMotion>();
	if (tiltMotion)
	{
		tiltMotion->SetIsActive(true);
	}

	gameObject.lock()->transform->SetLocalPositionY(0.0f);

	m_isAttack = false;
	m_vlp_attackFlashTimer->Stop();
	m_vlp_attackFlashTimer->Reset();
}

void ButiEngine::Worker::StopVibrationEffect()
{
	if (m_vwp_vibrationEffect.lock() != nullptr)
	{
		m_vwp_vibrationEffect.lock()->SetIsRemove(true);
		m_vwp_vibrationEffect = Value_weak_ptr<GameObject>();
	}
}

void ButiEngine::Worker::ShakeDrawObject()
{
	//if (!m_vwp_shakeComponent.lock())
	//{
	//	m_vwp_shakeComponent = m_vwp_tiltFloatObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock()->GetGameComponent<ShakeComponent>();
	//	m_vwp_shakeComponent.lock()->ShakeStart();
	//	m_vwp_shakeComponent.lock()->SetPositionAmplitude(2.0f);
	//	return;
	//}

	//float shakePower = m_vibration / m_maxVibration;
	//m_vwp_shakeComponent.lock()->SetShakePower(shakePower * 4.0f);
}

void ButiEngine::Worker::StopShakeDrawObject()
{
	if (!m_vwp_shakeComponent.lock())
	{
		m_vwp_shakeComponent = m_vwp_tiltFloatObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock()->GetGameComponent<ShakeComponent>();
		m_vwp_shakeComponent.lock()->ShakeStart();
		return;
	}
	m_vwp_shakeComponent.lock()->SetShakePower(0.0f);
}

void ButiEngine::Worker::IncreaseVibration()
{
	m_vibration += m_vibrationIncrease * GameDevice::WorldSpeed;
	m_vibration = min(m_vibration, m_maxVibration);
}

void ButiEngine::Worker::DecreaseVibration()
{
	if (m_isTutorialVibrationObject)
	{
		return;
	}
	m_vibration -= m_vibrationDecrease * GameDevice::WorldSpeed;
	m_vibration = max(m_vibration, m_minVibration);
}

void ButiEngine::Worker::ScaleAnimation()
{
	auto drawObject = m_vwp_tiltFloatObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject();
	if (!drawObject.lock()) { return; }
	if (!m_vwp_scaleAnimationComponent.lock())
	{
		m_vwp_scaleAnimationComponent = drawObject.lock()->GetGameComponent<EnemyScaleAnimationComponent>();
		return;
	}

	//???????X?P?[???????l???Z?b?g??????????
	float lerpScale = m_vibration / m_maxVibration;
	m_vwp_scaleAnimationComponent.lock()->SetScaleRate(lerpScale);
	m_vwp_scaleAnimationComponent.lock()->SetMaxPlusScale(m_maxScaleRate - 1.0f);
}

void ButiEngine::Worker::CreateDrawObject()
{
	m_vwp_tiltFloatObject = GetManager().lock()->AddObjectFromCereal("TiltFloatObject");
	m_vwp_tiltFloatObject.lock()->SetObjectName(m_vwp_tiltFloatObject.lock()->GetGameObjectName() + "_" + gameObject.lock()->GetGameObjectName());
	m_vwp_tiltFloatObject.lock()->GetGameComponent<TiltMotion>()->SetParent(gameObject);
	auto drawObject = m_vwp_tiltFloatObject.lock()->GetGameComponent<SeparateDrawObject>()->CreateDrawObject("Worker");
	drawObject.lock()->GetGameComponent<WingAnimation>()->SetParent(gameObject);
}

void ButiEngine::Worker::SetLookAtParameter()
{
	m_vlp_lookAt = gameObject.lock()->GetGameComponent<LookAtComponent>();
	m_vlp_lookAt->SetLookTarget(gameObject.lock()->transform->Clone());
	m_vlp_lookAt->GetLookTarget()->Translate(gameObject.lock()->transform->GetFront() * 100.0f);
	m_vlp_lookAt->SetSpeed(0.1f);
	if (m_isTutorialVibrationObject)
	{
		m_vlp_lookAt->SetIsActive(false);
	}
}

void ButiEngine::Worker::SetVibrationParameter()
{
	m_vibration = m_minVibration;
	if (m_isTutorialVibrationObject)
	{
		m_tutorialMaxVibration = 25.0f;
		if (gameObject.lock()->HasGameObjectTag("VibrationObject75"))
		{
			m_tutorialMaxVibration = 75.0f;
		}
		m_vibration = m_tutorialMaxVibration;
	}
}
