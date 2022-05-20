#include "stdafx_u.h"
#include "Enemy_Stalker.h"
#include "Enemy.h"
#include "Worker.h"
#include "SphereExclusion.h"
#include "SeparateDrawObject.h"

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
	m_maxMoveSpeed = 0.15f;
	m_acceleration = 0.01f;

	m_vlp_preyTimer = ObjectFactory::Create<RelativeTimer>(300);
	m_isPrey = false;
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
			worker->Dead();
		}
	}
}

void ButiEngine::Enemy_Stalker::OnCollisionPlayer(Value_weak_ptr<GameObject> arg_vwp_other)
{
	Prey(arg_vwp_other);
}

void ButiEngine::Enemy_Stalker::OnCollisionWorker(Value_weak_ptr<GameObject> arg_vwp_other)
{
	Prey(arg_vwp_other);
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

	gameObject.lock()->transform->SetLookAtRotation((arg_vwp_other.lock()->transform->GetWorldPosition()));
	m_vlp_lookAt->GetLookTarget()->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition() + gameObject.lock()->transform->GetFront());
	m_vlp_preyTimer->Start();
	m_isPrey = true;

	//Worker‚È‚ç•ß‚Ü‚¦‚é
	auto worker = arg_vwp_other.lock()->GetGameComponent<Worker>();
	if (worker)
	{
		m_vwp_preyTarget = arg_vwp_other;
		worker->Predated(gameObject);
	}
}

void ButiEngine::Enemy_Stalker::OnPrey()
{
	if (m_vlp_preyTimer->Update())
	{
		m_vlp_preyTimer->Stop();

		if (m_vwp_preyTarget.lock())
		{
			auto worker = m_vwp_preyTarget.lock()->GetGameComponent<Worker>();
			if (worker)
			{
				worker->Dead();
			}
		}

		m_isPrey = false;
	}
}

void ButiEngine::Enemy_Stalker::SetEnemyParameter()
{
	m_vlp_enemy = gameObject.lock()->GetGameComponent<Enemy>();
	m_vlp_enemy->CreatePocket(3);
	m_vlp_enemy->RemovePocket(0);
	m_vlp_enemy->SetVibrationCapacity(100.0f);
	m_vlp_enemy->SetVibrationResistance(0.0f);
	m_vlp_enemy->SetExplosionScale(3.0f);
	m_vlp_enemy->SetWeight(100.0f);
}

void ButiEngine::Enemy_Stalker::SetLookAtParameter()
{
	m_vlp_lookAt = gameObject.lock()->GetGameComponent<LookAtComponent>();
	m_vlp_lookAt->SetLookTarget(gameObject.lock()->transform->Clone());
	m_vlp_lookAt->GetLookTarget()->Translate(gameObject.lock()->transform->GetFront());
	m_vlp_lookAt->SetSpeed(0.1f);
}
