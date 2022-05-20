#include "stdafx_u.h"
#include "Sensor.h"
#include "Player.h"

void ButiEngine::Sensor::OnUpdate()
{
}

void ButiEngine::Sensor::OnSet()
{
	auto collisionStayLambda = std::function<void(Value_weak_ptr<GameObject>&)>([this](Value_weak_ptr<GameObject>& arg_vwp_other)->void
		{
			if (arg_vwp_other.lock()->GetIsRemove()) { return; }
			if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("Enemy")))
			{
				m_vlp_player->SetIsIncrease(true);
				m_vlp_player->AddNearEnemyCount();
			}
		});

	auto collisionLeaveLambda = std::function<void(Value_weak_ptr<GameObject>&)>([this](Value_weak_ptr<GameObject>& arg_vwp_other)->void
		{
			if (arg_vwp_other.lock()->GetIsRemove()) { return; }
			if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("Enemy")))
			{
				m_vlp_player->SetIsIncrease(false);
			}
		});

	gameObject.lock()->AddCollisionStayReaction(collisionStayLambda);
	gameObject.lock()->AddCollisionLeaveReaction(collisionLeaveLambda);
}

void ButiEngine::Sensor::Start()
{
	m_vlp_player = GetManager().lock()->GetGameObject(GameObjectTag("Player")).lock()->GetGameComponent<Player>();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Sensor::Clone()
{
	return ObjectFactory::Create<Sensor>();
}
