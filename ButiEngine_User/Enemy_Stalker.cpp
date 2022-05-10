#include "stdafx_u.h"
#include "Enemy_Stalker.h"
#include "Enemy.h"
#include "Worker.h"
#include "SphereExclusion.h"

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
	if (m_vlp_enemy)
	{
		m_vlp_enemy->Explosion();
	}
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
	SetEnemyParameter();

	gameObject.lock()->GetGameComponent<SphereExclusion>()->SetMass(90.0f);

	m_velocity = Vector3Const::Zero;
	m_maxMoveSpeed = 0.15f;
	m_acceleration = 0.01f;

	m_vlp_lookAt = gameObject.lock()->GetGameComponent<LookAtComponent>();
	m_vlp_lookAt->SetSpeed(0.1f);
	m_vlp_preyTimer = ObjectFactory::Create<RelativeTimer>(300);
	m_isPrey = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Enemy_Stalker::Clone()
{
	return ObjectFactory::Create<Enemy_Stalker>();
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
	m_velocity += dir.GetNormalize() * m_acceleration;
	if (m_velocity.GetLength() > m_maxMoveSpeed)
	{
		m_velocity = m_velocity.GetNormalize() * m_maxMoveSpeed;
	}

	gameObject.lock()->transform->Translate(m_velocity);


	auto lookTarget = gameObject.lock()->transform->Clone();
	lookTarget->Translate(dir);
	m_vlp_lookAt->SetLookTarget(lookTarget);
}

void ButiEngine::Enemy_Stalker::Prey(Value_weak_ptr<GameObject> arg_vwp_other)
{
	if (m_isPrey) { return; }

	//Worker‚È‚ç•ß‚Ü‚¦‚é
	if (arg_vwp_other.lock()->GetGameComponent<Worker>())
	{
		m_vwp_preyTarget = arg_vwp_other;
	}

	m_velocity = Vector3Const::Zero;

	m_vlp_lookAt->SetLookTarget(arg_vwp_other.lock()->transform->Clone());
	m_vlp_preyTimer->Start();
	m_isPrey = true;
}

void ButiEngine::Enemy_Stalker::OnPrey()
{
	if (m_vlp_preyTimer->Update())
	{
		m_vlp_preyTimer->Stop();

		if (m_vwp_preyTarget.lock())
		{
			m_vwp_preyTarget.lock()->SetIsRemove(true);
		}

		m_isPrey = false;
	}
}

void ButiEngine::Enemy_Stalker::SetEnemyParameter()
{
	m_vlp_enemy = gameObject.lock()->GetGameComponent<Enemy>();
	m_vlp_enemy->CreatePocket(3);
	m_vlp_enemy->SetNearBorder(gameObject.lock()->transform->GetLocalScale().x * 0.5f + 1.0f);
	m_vlp_enemy->SetVibrationCapacity(100.0f);
	m_vlp_enemy->SetVibrationResistance(3.0f);
	m_vlp_enemy->SetExplosionScale(3.0f);
}
