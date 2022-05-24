#include "stdafx_u.h"
#include "Enemy_Stalker.h"
#include "Enemy.h"
#include "Worker.h"
#include "SphereExclusion.h"
#include "SeparateDrawObject.h"

std::int32_t ButiEngine::Enemy_Stalker::m_pocketCount = 5;
float ButiEngine::Enemy_Stalker::m_createPocketRadius = 1.0f;

void ButiEngine::Enemy_Stalker::OnUpdate()
{
	if (m_isPrey)
	{
		OnPrey();
	}
	else
	{
		Move();
	}
}

void ButiEngine::Enemy_Stalker::OnSet()
{
	auto collisionLambda = std::function<void(Value_weak_ptr<GameObject>&)>([this](Value_weak_ptr<GameObject>& arg_vwp_other)->void
		{
			if (arg_vwp_other.lock()->GetIsRemove()) { return; }
			if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("Player")))
			{
				OnCollisionPlayer(arg_vwp_other);
			}
			else if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("Worker")))
			{
				OnCollisionWorker(arg_vwp_other);
			}
			else if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("Enemy")))
			{
				OnCollisionEnemy(arg_vwp_other);
			}
		});

	gameObject.lock()->AddCollisionEnterReaction(collisionLambda);
}

void ButiEngine::Enemy_Stalker::OnRemove()
{
}

void ButiEngine::Enemy_Stalker::OnShowUI()
{
	GUI::BulletText("PocketCount");
	GUI::DragInt("##pocketCount", &m_pocketCount, 1, 0, 64);
	GUI::BulletText("PocketRadius");
	GUI::DragFloat("##radius", &m_createPocketRadius, 0.1f, 0.0f, 100.0f);

	GUI::BulletText("MaxMoveSpeed");
	GUI::DragFloat("##moveSpeed", &m_maxMoveSpeed, 0.01f, 0.0f, 1.0f);

	GUI::BulletText("Acceleration");
	GUI::DragFloat("##accel", &m_acceleration, 0.001f, 0.0f, 1.0f);
}

void ButiEngine::Enemy_Stalker::Start()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->CreateDrawObject("Stalker");

	SetEnemyParameter();
	SetLookAtParameter();

	gameObject.lock()->GetGameComponent<SphereExclusion>()->SetWeight(100.0f);

	m_velocity = Vector3Const::Zero;
	m_maxMoveSpeed = 0.125f;
	m_acceleration = 0.01f;

	m_vlp_preyTimer = ObjectFactory::Create<RelativeTimer>(300);
	m_isPrey = false;

	m_lookTargetAnimationStartPos = Vector3Const::Zero;
	m_lookTargetMoveDir = gameObject.lock()->transform->GetRight();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Enemy_Stalker::Clone()
{
	return ObjectFactory::Create<Enemy_Stalker>();
}

void ButiEngine::Enemy_Stalker::Dead()
{
	if (m_vlp_enemy)
	{
		m_vlp_enemy->Explosion();
	}

	if (m_vwp_preyTarget.lock())
	{
		auto worker = m_vwp_preyTarget.lock()->GetGameComponent<Worker>();
		if (worker)
		{
			Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
			Vector3 workerPos = m_vwp_preyTarget.lock()->transform->GetWorldPosition();
			Vector3 dir = (workerPos - pos).GetNormalize();
			worker->Rupture(dir);
		}
	}
}

void ButiEngine::Enemy_Stalker::OnCollisionPlayer(Value_weak_ptr<GameObject> arg_vwp_other)
{
	Prey(arg_vwp_other);
}

void ButiEngine::Enemy_Stalker::OnCollisionWorker(Value_weak_ptr<GameObject> arg_vwp_other)
{
	//Prey(arg_vwp_other);
}

void ButiEngine::Enemy_Stalker::OnCollisionEnemy(Value_weak_ptr<GameObject> arg_vwp_other)
{
}

void ButiEngine::Enemy_Stalker::Move()
{
	Vector3 dir = m_vlp_enemy->GetPlayer().lock()->transform->GetLocalPosition() - gameObject.lock()->transform->GetLocalPosition();
	m_velocity += dir.GetNormalize() * m_acceleration * GameDevice::WorldSpeed;
	if (m_velocity.GetLength() > m_maxMoveSpeed)
	{
		m_velocity = m_velocity.GetNormalize() * m_maxMoveSpeed;
	}

	gameObject.lock()->transform->Translate(m_velocity * GameDevice::WorldSpeed);
	m_vlp_lookAt->GetLookTarget()->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition() + dir);
}

void ButiEngine::Enemy_Stalker::Prey(Value_weak_ptr<GameObject> arg_vwp_other)
{
	if (m_isPrey) { return; }

	m_velocity = Vector3Const::Zero;

	m_vlp_lookAt->GetLookTarget()->SetLocalPosition(arg_vwp_other.lock()->transform->GetWorldPosition());
	gameObject.lock()->transform->SetLookAtRotation(arg_vwp_other.lock()->transform->GetWorldPosition());
	m_vlp_lookAt->SetSpeed(1.0f);

	m_vlp_preyTimer->Start();
	m_isPrey = true;

	//Workerなら捕まえる
	auto worker = arg_vwp_other.lock()->GetGameComponent<Worker>();
	if (worker)
	{
		m_vwp_preyTarget = arg_vwp_other;

		m_lookTargetAnimationStartPos = m_vlp_lookAt->GetLookTarget()->GetLocalPosition();
		m_lookTargetMoveDir = gameObject.lock()->transform->GetRight();

		worker->Predated(gameObject);
	}
}

void ButiEngine::Enemy_Stalker::OnPrey()
{
	//モブハチを捕食していた時アニメーションする
	if (m_vwp_preyTarget.lock())
	{
		PreyAnimation();
	}

	if (m_vlp_preyTimer->Update())
	{
		m_vlp_preyTimer->Stop();

		m_vlp_lookAt->SetSpeed(m_defaultLookSpeed);
		m_vlp_lookAt->GetLookTarget()->SetLocalPosition(m_lookTargetAnimationStartPos);

		//モブハチ殺す
		if (m_vwp_preyTarget.lock())
		{
			auto worker = m_vwp_preyTarget.lock()->GetGameComponent<Worker>();
			if (worker)
			{
				Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
				Vector3 workerPos = m_vwp_preyTarget.lock()->transform->GetWorldPosition();
				Vector3 dir = (workerPos - pos).GetNormalize();
				worker->Rupture(dir);
			}
		}

		m_isPrey = false;
	}
}

void ButiEngine::Enemy_Stalker::PreyAnimation()
{
	float progress = m_vlp_preyTimer->GetPercent() * GameDevice::WorldSpeed;
	Vector3 newLookTargetPos = m_lookTargetAnimationStartPos + m_lookTargetMoveDir * (sin(2 * PI * progress * 10.0f)) * 1.5f;
	m_vlp_lookAt->GetLookTarget()->SetLocalPosition(newLookTargetPos);
}

void ButiEngine::Enemy_Stalker::SetEnemyParameter()
{
	m_vlp_enemy = gameObject.lock()->GetGameComponent<Enemy>();
	m_vlp_enemy->CreatePocket(m_pocketCount, m_createPocketRadius);
	m_vlp_enemy->SetVibrationCapacity(30.0f);
	m_vlp_enemy->SetVibrationResistance(0.0f);
	m_vlp_enemy->SetExplosionScale(3.0f);
	m_vlp_enemy->SetWeight(100.0f);
}

void ButiEngine::Enemy_Stalker::SetLookAtParameter()
{
	m_defaultLookSpeed = 0.1f;

	m_vlp_lookAt = gameObject.lock()->GetGameComponent<LookAtComponent>();
	m_vlp_lookAt->SetLookTarget(gameObject.lock()->transform->Clone());
	m_vlp_lookAt->GetLookTarget()->Translate(gameObject.lock()->transform->GetFront());
	m_vlp_lookAt->SetSpeed(m_defaultLookSpeed);
}
