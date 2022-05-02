#include "stdafx_u.h"
#include "Worker.h"
#include "Enemy.h"
#include "Pocket.h"
#include "Stick.h"
#include "ButiBulletWrap/ButiBulletWrap/Common.h"

void ButiEngine::Worker::OnUpdate()
{
}

void ButiEngine::Worker::OnSet()
{
	gameObject.lock()->AddCollisionStayReaction(std::function<void(ButiBullet::ContactData&)>([this](ButiBullet::ContactData& arg_other)->void 
		{
			if (arg_other.vwp_gameObject.lock()->HasGameObjectTag(GameObjectTag("Enemy")))
			{
				OnCollisionEnemy(arg_other.vwp_gameObject);
			}
		}));
}

void ButiEngine::Worker::Start()
{
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
	auto pocket = enemyComponent->GetFreePocket();

	if (pocket.lock())
	{
		gameObject.lock()->RemoveGameComponent("Flocking");

		auto stickComponent = gameObject.lock()->AddGameComponent<Stick>();
		stickComponent->SetPocket(pocket);
	}
}
