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
#include "ButiBulletWrap/ButiBulletWrap/Common.h"

float ButiEngine::Worker::m_nearBorder = 3.0f;
float ButiEngine::Worker::m_vibrationForce = 1.0f;

void ButiEngine::Worker::OnUpdate()
{

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
			if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("Stalker")))
			{
				OnCollisionStalker(arg_vwp_other);
			}
			else if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("Enemy")))
			{
				OnCollisionEnemy(arg_vwp_other);
			}
			else if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("DamageArea")))
			{

				Dead();
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

	gameObject.lock()->GetGameComponent<SphereExclusion>()->SetMass(1.0f);

	SetLookAtParameter();

	m_defaultScale = gameObject.lock()->transform->GetLocalScale();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Worker::Clone()
{
	return ObjectFactory::Create<Worker>();
}

void ButiEngine::Worker::Dead()
{
	//m_vwp_beeSoul = GetManager().lock()->AddObjectFromCereal("BeeSoul");
	//m_vwp_beeSoul.lock()->GetGameComponent<BeeSoulComponent>()->SetPosition(gameObject.lock()->transform->GetWorldPosition());

	auto player = GetManager().lock()->GetGameObject(GameObjectTag("Player")).lock()->GetGameComponent<Player>();
	if (player)
	{
		player->AddExp();
	}

	auto workerSpawner = GetManager().lock()->GetGameObject(GameObjectTag("WorkerSpawner")).lock()->GetGameComponent<WorkerSpawner>();
	if (workerSpawner)
	{
		workerSpawner->StartTimer();
	}

	m_vwp_tiltFloatObject.lock()->GetGameComponent<SeparateDrawObject>()->Dead();

	StopVibrationEffect();

	gameObject.lock()->SetIsRemove(true);
}

void ButiEngine::Worker::OnCollisionStalker(Value_weak_ptr<GameObject> arg_vwp_other)
{
	auto stalker = arg_vwp_other.lock()->GetGameComponent<Enemy_Stalker>();
	if (!stalker) { return; }

	//捕食中ならしがみつく
	bool isPrey = stalker->IsPrey();
	if (isPrey)
	{
		OnCollisionEnemy(arg_vwp_other);
		return; 
	}

	auto flocking = gameObject.lock()->GetGameComponent<Flocking>();
	if (flocking)
	{
		flocking->SetIsRemove(true);
	}

	auto moveRestriction = gameObject.lock()->GetGameComponent<MoveRestriction>();
	if (moveRestriction)
	{
		moveRestriction->SetIsRemove(true);
	}

	auto collider = gameObject.lock()->GetGameComponent<Collision::ColliderComponent>();
	if (collider)
	{
		collider->SetIsRemove(true);
	}
}

void ButiEngine::Worker::OnCollisionEnemy(Value_weak_ptr<GameObject> arg_vwp_enemy)
{
	//既にしがみついていたら何もしない
	if (gameObject.lock()->GetGameComponent<Stick>()) { return; }

	//敵に空いているポケットがあったらしがみつく
	auto enemyComponent = arg_vwp_enemy.lock()->GetGameComponent<Enemy>();
	if (!enemyComponent) { return; }
	auto pocket = enemyComponent->GetNearFreePocket(gameObject.lock()->transform->GetLocalPosition(), m_nearBorder);

	if (pocket.lock())
	{
		auto flocking = gameObject.lock()->GetGameComponent<Flocking>();
		if (flocking)
		{
			flocking->SetIsRemove(true);
		}

		auto moveRestriction = gameObject.lock()->GetGameComponent<MoveRestriction>();
		if (moveRestriction)
		{
			moveRestriction->SetIsRemove(true);
		}

		auto collider = gameObject.lock()->GetGameComponent<Collision::ColliderComponent>();
		if (collider)
		{
			collider->SetIsRemove(true);
		}

		m_vlp_lookAt->SetLookTarget(arg_vwp_enemy.lock()->transform);


		auto stickComponent = gameObject.lock()->AddGameComponent<Stick>();
		stickComponent->SetPocket(pocket);
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
