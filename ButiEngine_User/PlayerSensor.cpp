#include "stdafx_u.h"
#include "PlayerSensor.h"
#include "Enemy.h"
#include "Player.h"

void ButiEngine::PlayerSensor::OnUpdate()
{
}

void ButiEngine::PlayerSensor::OnSet()
{
	auto collisionStayLambda = std::function<void(Value_weak_ptr<GameObject>&)>([this](Value_weak_ptr<GameObject>& arg_vwp_other)->void
		{
			if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("Player")))
			{
				m_vlp_player->SetIsIncrease(true);
				m_vlp_player->AddNearEnemyCount();
				m_vlp_enemy->SetIsNearPlayer(true);
			}
		});

	auto collisionLeaveLambda = std::function<void(Value_weak_ptr<GameObject>&)>([this](Value_weak_ptr<GameObject>& arg_vwp_other)->void
		{
			if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("Player")))
			{
				m_vlp_player->SetIsIncrease(false);
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
	m_vlp_player->SetIsIncrease(false);
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
