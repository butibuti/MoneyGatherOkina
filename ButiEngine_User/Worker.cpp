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
#include "ButiBulletWrap/ButiBulletWrap/Common.h"

float ButiEngine::Worker::m_nearBorder = 3.0f;
float ButiEngine::Worker::m_vibrationForce = 1.0f;

void ButiEngine::Worker::OnUpdate()
{
}

void ButiEngine::Worker::OnSet()
{
	auto collisionLambda = std::function<void(Value_weak_ptr<GameObject>&)>([this](Value_weak_ptr<GameObject>& arg_vwp_other)->void
		{
			if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("Enemy")))
			{
				OnCollisionEnemy(arg_vwp_other);
			}
			else if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("DamageArea")))
			{
				gameObject.lock()->SetIsRemove(true);
			}
		});

	gameObject.lock()->AddCollisionEnterReaction(collisionLambda);
	gameObject.lock()->AddCollisionStayReaction(collisionLambda);
}

void ButiEngine::Worker::OnRemove()
{
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
	gameObject.lock()->GetGameComponent<SphereExclusion>()->SetMass(0.1f);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Worker::Clone()
{
	return ObjectFactory::Create<Worker>();
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
