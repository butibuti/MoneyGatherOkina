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
			m_vwp_vibrationEffect = GetManager().lock()->AddObjectFromCereal("VibrationEffect");
			m_vwp_vibrationEffect.lock()->transform->SetLocalPosition(transform->GetWorldPosition());
			m_vwp_vibrationEffect.lock()->transform->SetLocalScale(m_defaultScale * 2.0f);

			m_vwp_vibrationEffectComponent = m_vwp_vibrationEffect.lock()->GetGameComponent<VibrationEffectComponent>();
		}
		else
		{
			auto transform = gameObject.lock()->transform;
			m_vwp_vibrationEffectComponent.lock()->SetVibration();
			m_vwp_vibrationEffectComponent.lock()->SetEffectPosition(transform->GetWorldPosition());
		}
	}
	else
	{
		StopVibrationEffect();
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
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->CreateDrawObject("Worker");
	gameObject.lock()->GetGameComponent<SphereExclusion>()->SetMass(0.1f);

	m_defaultScale = gameObject.lock()->transform->GetLocalScale();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Worker::Clone()
{
	return ObjectFactory::Create<Worker>();
}

void ButiEngine::Worker::Dead()
{
	auto beeSoul = GetManager().lock()->AddObjectFromCereal("BeeSoul");
	Vector3 screenPosition = GetCamera("main")->WorldToScreen(gameObject.lock()->transform->GetWorldPosition());
	screenPosition.z = 0;
	beeSoul.lock()->transform->SetLocalPosition(screenPosition);

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

	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->Dead();

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
