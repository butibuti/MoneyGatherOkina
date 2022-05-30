#include "stdafx_u.h"
#include "Sensor.h"
#include "Player.h"
#include "Worker.h"

void ButiEngine::Sensor::OnUpdate()
{
}

void ButiEngine::Sensor::OnSet()
{
	auto collisionStayLambda = std::function<void(Value_ptr<GameObject>&)>([this](Value_ptr<GameObject>& arg_vwp_other)->void
		{
			if (arg_vwp_other->GetIsRemove()) { return; }
			if (arg_vwp_other->HasGameObjectTag(GameObjectTag("Enemy")))
			{
				//m_vwp_player.lock()->AddNearEnemyCount();
			}
			else if (arg_vwp_other->HasGameObjectTag(GameObjectTag("Flocking")))
			{
				OnCollisionFlocking(arg_vwp_other);
			}
		});

	auto collisionLeaveLambda = std::function<void(Value_ptr<GameObject>&)>([this](Value_ptr<GameObject>& arg_vwp_other)->void
		{
		});

	gameObject.lock()->AddCollisionStayReaction(collisionStayLambda);
	gameObject.lock()->AddCollisionLeaveReaction(collisionLeaveLambda);
}

void ButiEngine::Sensor::Start()
{
	m_vwp_player = GetManager().lock()->GetGameObject(GameObjectTag("Player")).lock()->GetGameComponent<Player>();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Sensor::Clone()
{
	return ObjectFactory::Create<Sensor>();
}

void ButiEngine::Sensor::OnCollisionFlocking(Value_weak_ptr<GameObject> arg_vwp_other)
{
	//吸収するやつ
	//auto worker = arg_vwp_other.lock()->GetGameComponent<Worker>();
	//if (!worker) { return; }

	////モブハチの振動値がプレイヤーの振動値の上昇量より大きかったら振動値を増やす
	//float playerVibrationIncrease = m_vwp_player.lock()->GetVibrationIncrease();
	//float workerVibration = worker->GetVibration();

	//if (workerVibration > playerVibrationIncrease)
	//{
	//	m_vwp_player.lock()->AddNearWorker(worker);
	//}

	//m_vwp_player.lock()->SetStrongestNearWorkerVibration(workerVibration);

	auto worker = arg_vwp_other.lock()->GetGameComponent<Worker>();
	if (!worker) { return; }

	//プレイヤーの振動値がモブハチの振動値より小さかったら振動値を増やす
	float playerVibration = m_vwp_player.lock()->GetVibration();
	float workerVibration = worker->GetVibration();

	if (playerVibration <= workerVibration)
	{
		m_vwp_player.lock()->AddNearWorker(worker);
	}

	m_vwp_player.lock()->SetStrongestNearWorkerVibration(workerVibration);
}
