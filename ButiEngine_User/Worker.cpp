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
#include "TiltMotion.h"
#include "FloatMotionComponent.h"
#include "WorkerAttackFlashEffect.h"
#include "KnockBack.h"
#include "ButiBulletWrap/ButiBulletWrap/Common.h"

float ButiEngine::Worker::m_nearBorder = 1.0f;
float ButiEngine::Worker::m_vibrationForce = 1.0f;

void ButiEngine::Worker::OnUpdate()
{
	if (m_isNearPlayer)
	{
		OnNearPlayer();
	}
	if (m_isRupture)
	{
		OnRupture();
	}

	if (m_isVibration)
	{
		if (m_vwp_vibrationEffect.lock() == nullptr)
		{
			auto transform = gameObject.lock()->transform;
			m_vwp_vibrationEffect = GetManager().lock()->AddObjectFromCereal("VibrationEffect_Player");
			m_vwp_vibrationEffect.lock()->transform->SetLocalPosition(transform->GetWorldPosition());
			m_vwp_vibrationEffect.lock()->transform->SetLocalScale(m_defaultScale * 4.0f);

			m_vwp_vibrationEffectComponent = m_vwp_vibrationEffect.lock()->GetGameComponent<VibrationEffectComponent>();
		}
		else
		{
			auto transform = gameObject.lock()->transform;
			m_vwp_vibrationEffectComponent.lock()->SetVibration();
			m_vwp_vibrationEffectComponent.lock()->SetEffectPosition(transform->GetWorldPosition());
		}
		ShakeDrawObject();
	}
	else
	{
		StopVibrationEffect();
		StopShakeDrawObject();
	}
}

void ButiEngine::Worker::OnSet()
{
	auto collisionLambda = std::function<void(Value_weak_ptr<GameObject>&)>([this](Value_weak_ptr<GameObject>& arg_vwp_other)->void
		{
			if (arg_vwp_other.lock()->GetIsRemove()) { return; }
			if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("Sensor")))
			{
				OnCollisionPlayer(arg_vwp_other);
			}
			//else if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("Stalker")))
			//{
			//	OnCollisionStalker(arg_vwp_other);
			//}
			else if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("Enemy")))
			{
				OnCollisionEnemy(arg_vwp_other);
			}
			else if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("DamageArea")))
			{
				Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
				Vector3 damageAreaPos = arg_vwp_other.lock()->transform->GetWorldPosition();
				Vector3 dir = (pos - damageAreaPos).GetNormalize();
				Rupture(dir);
			}
		});

	m_vwp_particleGenerater = GetManager().lock()->GetGameObject("BillBoardParticleController").lock()->GetGameComponent<ParticleGenerater>();

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
	GUI::BulletText("VibrationForce");
	GUI::DragFloat("##vForce", &m_vibrationForce, 1.0f, 0.0f, 100.0f);
}

void ButiEngine::Worker::Start()
{
	CreateDrawObject();

	gameObject.lock()->GetGameComponent<SphereExclusion>()->SetWeight(1.0f);

	SetLookAtParameter();

	m_defaultScale = gameObject.lock()->transform->GetLocalScale();
	m_isPredated = false;

	m_vlp_player = GetManager().lock()->GetGameObject(GameObjectTag("Player")).lock()->GetGameComponent<Player>();

	auto spawnFire = GetManager().lock()->AddObjectFromCereal("MobSpawnFire");
	spawnFire.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition());
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Worker::Clone()
{
	return ObjectFactory::Create<Worker>();
}

void ButiEngine::Worker::Dead()
{
	m_vwp_beeSoul = GetManager().lock()->AddObjectFromCereal("BeeSoul");
	m_vwp_beeSoul.lock()->GetGameComponent<BeeSoulComponent>()->SetPosition(gameObject.lock()->transform->GetWorldPosition());

	if (m_vlp_player)
	{
		m_vlp_player->AddExp();
	}

	auto workerSpawner = GetManager().lock()->GetGameObject(GameObjectTag("WorkerSpawner")).lock()->GetGameComponent<WorkerSpawner>();
	if (workerSpawner)
	{
		workerSpawner->StartTimer();
	}

	auto stick = gameObject.lock()->GetGameComponent<Stick>();
	if (stick)
	{
		stick->Dead();
		stick->SetIsRemove(true);
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
	gameObject.lock()->transform->SetBaseTransform(nullptr);

	auto stick = gameObject.lock()->GetGameComponent<Stick>();
	if (stick)
	{
		stick->Dead();
		stick->SetIsRemove(true);
	}

	auto tiltMotion = m_vwp_tiltFloatObject.lock()->GetGameComponent<TiltMotion>();
	if (tiltMotion)
	{
		tiltMotion->SetIsRemove(true);
	}

	auto floatMotion = m_vwp_tiltFloatObject.lock()->GetGameComponent<FloatMotionComponent>();
	if (floatMotion)
	{
		floatMotion->SetIsRemove(true);
	}

	if (m_vwp_attackFlash.lock())
	{
		m_vwp_attackFlash.lock()->GetGameComponent<WorkerAttackFlashEffect>()->Dead();
	}

	m_isVibration = false;
	StopVibrationEffect();

	std::int8_t frame = 30;

	Vector3 dir = arg_dir;
	dir.y = 2.0f;
	dir.Normalize();
	gameObject.lock()->AddGameComponent<KnockBack>(dir, 0.9f, true, frame);
	m_vlp_ruptureTimer = ObjectFactory::Create<RelativeTimer>(frame);
	m_vlp_ruptureTimer->Start();

	m_isRupture = true;
}

void ButiEngine::Worker::Predated(Value_weak_ptr<GameObject> arg_vwp_other)
{
	//捕食されていたら何もしない
	if (m_isPredated) { return; }

	auto flocking = gameObject.lock()->GetGameComponent<Flocking>();
	if (flocking)
	{
		flocking->SetIsRemove(true);
	}

	if (m_vlp_lookAt)
	{
		m_vlp_lookAt->SetIsRemove(true);
	}

	auto collider = gameObject.lock()->GetGameComponent<Collision::ColliderComponent>();
	if (collider)
	{
		collider->SetIsRemove(true);
	}

	auto tiltMotion = m_vwp_tiltFloatObject.lock()->GetGameComponent<TiltMotion>();
	if (tiltMotion)
	{
		tiltMotion->SetIsRemove(true);
	}

	auto floatMotion = m_vwp_tiltFloatObject.lock()->GetGameComponent<FloatMotionComponent>();
	if (floatMotion)
	{
		floatMotion->SetIsRemove(true);
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

void ButiEngine::Worker::CreateAttackFlash(const Vector3& arg_pos)
{
	if (m_vwp_attackFlash.lock()) { return; }

	m_vwp_attackFlash = GetManager().lock()->AddObjectFromCereal("WorkerAttackFlashEffect");
	m_vwp_attackFlash.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	m_vwp_attackFlash.lock()->transform->SetWorldPosition(arg_pos);
}

void ButiEngine::Worker::OnCollisionPlayer(Value_weak_ptr<GameObject> arg_vwp_other)
{
	if (m_isNearPlayer) { return; }

	float playerVibrationRate = m_vlp_player->GetVibrationRate();
	if (playerVibrationRate >= 1.0f)
	{
		auto flocking = gameObject.lock()->GetGameComponent<Flocking>();
		if (flocking)
		{
			flocking->SetIsRemove(true);
		}

		auto collider = gameObject.lock()->GetGameComponent<Collision::ColliderComponent>();
		if (collider)
		{
			collider->SetIsRemove(true);
		}

		m_vlp_nearPlayerTimer = ObjectFactory::Create<RelativeTimer>(30);
		m_vlp_nearPlayerTimer->Start();
		m_isNearPlayer = true;
		m_isVibration = true;
	}
}

void ButiEngine::Worker::OnCollisionStalker(Value_weak_ptr<GameObject> arg_vwp_other)
{
	//捕食されていたら何もしない
	if (m_isPredated) { return; }

	auto stalker = arg_vwp_other.lock()->GetGameComponent<Enemy_Stalker>();
	if (!stalker) { return; }

	//捕食中ならしがみつく
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
	//捕食されていたら何もしない
	if (m_isPredated) { return; }
	//既にしがみついていたら何もしない
	if (gameObject.lock()->GetGameComponent<Stick>()) { return; }

	//敵に空いているポケットがあったらしがみつく
	auto enemyComponent = arg_vwp_enemy.lock()->GetGameComponent<Enemy>();
	if (!enemyComponent) { return; }
	auto pocket = enemyComponent->GetNearFreePocket(gameObject.lock()->transform->GetLocalPosition(), m_nearBorder);

	if (pocket.lock())
	{
		gameObject.lock()->transform->SetLookAtRotation(arg_vwp_enemy.lock()->transform->GetLocalPosition());

		enemyComponent->AddStickWorkerCount();

		auto flocking = gameObject.lock()->GetGameComponent<Flocking>();
		if (flocking)
		{
			flocking->SetIsRemove(true);
		}

		auto collider = gameObject.lock()->GetGameComponent<Collision::ColliderComponent>();
		if (collider)
		{
			collider->SetIsRemove(true);
		}

		auto lookAt = gameObject.lock()->GetGameComponent<LookAtComponent>();
		if (lookAt)
		{
			lookAt->SetIsRemove(true);
		}

		auto tiltMotion = m_vwp_tiltFloatObject.lock()->GetGameComponent<TiltMotion>();
		if (tiltMotion)
		{
			tiltMotion->SetIsRemove(true);
		}

		//auto floatMotion = m_vwp_tiltFloatObject.lock()->GetGameComponent<FloatMotionComponent>();
		//if (floatMotion)
		//{
		//	//floatMotion->SetIsRemove(true);
		//}

		Vector3 pos = gameObject.lock()->transform->GetWorldPosition();
		float radius = gameObject.lock()->transform->GetWorldScale().x * 0.5f;
		Vector3 dir = (arg_vwp_enemy.lock()->transform->GetLocalPosition() - pocket.lock()->transform->GetWorldPosition()).GetNormalize();
		gameObject.lock()->GetGameComponent<Worker>()->CreateAttackFlash(pos + dir * radius);


		auto stickComponent = gameObject.lock()->AddGameComponent<Stick>();
		stickComponent->SetPocket(pocket);
	}
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
	if (m_vlp_ruptureTimer->Update())
	{
		Dead();
	}
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
	if (!m_vwp_shakeComponent.lock())
	{
		m_vwp_shakeComponent = m_vwp_tiltFloatObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock()->GetGameComponent<ShakeComponent>();
		m_vwp_shakeComponent.lock()->ShakeStart();
		return;
	}
	m_vwp_shakeComponent.lock()->SetShakePower(1.0f);
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

void ButiEngine::Worker::CreateDrawObject()
{
	m_vwp_tiltFloatObject = GetManager().lock()->AddObjectFromCereal("TiltFloatObject");
	m_vwp_tiltFloatObject.lock()->SetObjectName(m_vwp_tiltFloatObject.lock()->GetGameObjectName() + "_" + gameObject.lock()->GetGameObjectName());
	m_vwp_tiltFloatObject.lock()->GetGameComponent<TiltMotion>()->SetParent(gameObject);
	m_vwp_tiltFloatObject.lock()->GetGameComponent<SeparateDrawObject>()->CreateDrawObject("Worker");
}

void ButiEngine::Worker::SetLookAtParameter()
{
	m_vlp_lookAt = gameObject.lock()->GetGameComponent<LookAtComponent>();
	m_vlp_lookAt->SetLookTarget(gameObject.lock()->transform->Clone());
	m_vlp_lookAt->SetSpeed(0.1f);
}
