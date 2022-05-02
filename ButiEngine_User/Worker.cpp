#include "stdafx_u.h"
#include "Worker.h"
#include "Enemy.h"
#include "Pocket.h"
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
	auto enemyComponent = arg_vwp_enemy.lock()->GetGameComponent<Enemy>();
	auto pocket = enemyComponent->GetFreePocket();

	//gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color = ButiColor::Red();
	if (pocket.lock())
	{
		auto pocketComponent = pocket.lock()->GetGameComponent<Pocket>();
		pocketComponent->SetWorker(gameObject);
	}
}
