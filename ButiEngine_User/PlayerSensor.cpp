#include "stdafx_u.h"
#include "PlayerSensor.h"
#include "Enemy.h"
#include "Player.h"

void ButiEngine::PlayerSensor::OnUpdate()
{
}

void ButiEngine::PlayerSensor::OnSet()
{
	auto collisionStayLambda = std::function<void(Value_ptr<GameObject>&)>([this](Value_ptr<GameObject>& arg_vwp_other)->void
		{
			if (arg_vwp_other->GetIsRemove()) { return; }
			if (arg_vwp_other->HasGameObjectTag(GameObjectTag("Player")))
			{
				float nearEnemyVibrationRate = m_vlp_player->GetNearEnemyVibrationRate();
				float enemyVibrationRate = m_vlp_enemy->GetVibrationRate();
				if (enemyVibrationRate > nearEnemyVibrationRate)
				{
					m_vlp_player->SetNearEnemyVibrationRate(enemyVibrationRate);
				}

				m_vlp_enemy->SetIsNearPlayer(true);
			}
		});

	auto collisionLeaveLambda = std::function<void(Value_ptr<GameObject>&)>([this](Value_ptr<GameObject>& arg_vwp_other)->void
		{
			if (arg_vwp_other->GetIsRemove()) { return; }
			if (arg_vwp_other->HasGameObjectTag(GameObjectTag("Player")))
			{
				m_vlp_player->SetNearEnemyVibrationRate(0.0f);
				m_vlp_enemy->SetIsNearPlayer(false);
			}
		});

	gameObject.lock()->AddCollisionStayReaction(collisionStayLambda);
	gameObject.lock()->AddCollisionLeaveReaction(collisionLeaveLambda);

	m_vwp_player = GetManager().lock()->GetGameObject(GameObjectTag("Player"));
	m_vlp_player = m_vwp_player.lock()->GetGameComponent<Player>();
}

void ButiEngine::PlayerSensor::OnRemove()
{
}

void ButiEngine::PlayerSensor::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::PlayerSensor::Clone()
{
	return ObjectFactory::Create<PlayerSensor>();
}

void ButiEngine::PlayerSensor::SetParentEnemy(Value_weak_ptr<GameObject> arg_vwp_enemy)
{
	m_vwp_enemy = arg_vwp_enemy;
	m_vlp_enemy = m_vwp_enemy.lock()->GetGameComponent<Enemy>();
}

void ButiEngine::PlayerSensor::Dead()
{
	m_vlp_player->SetNearEnemyVibrationRate(0.0f);

	gameObject.lock()->SetIsRemove(true);
}
