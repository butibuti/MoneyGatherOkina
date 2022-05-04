#include "stdafx_u.h"
#include "Worker.h"
#include "Enemy.h"
#include "Pocket.h"
#include "Stick.h"
#include "ButiBulletWrap/ButiBulletWrap/Common.h"

float ButiEngine::Worker::m_nearBorder;

void ButiEngine::Worker::OnUpdate()
{
}

void ButiEngine::Worker::OnSet()
{
	m_nearBorder = 1.0f;

	gameObject.lock()->AddCollisionStayReaction(std::function<void(ButiBullet::ContactData&)>([this](ButiBullet::ContactData& arg_other)->void 
		{
			if (arg_other.vwp_gameObject.lock()->HasGameObjectTag(GameObjectTag("Enemy")))
			{
				OnCollisionEnemy(arg_other.vwp_gameObject);
			}
		}));
}

void ButiEngine::Worker::OnShowUI()
{
	GUI::BulletText("NearBorder");
	GUI::DragFloat("##nearBorder", &m_nearBorder, 0.1f, 0.0f, 10.0f);
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
	auto pocket = enemyComponent->GetNearFreePocket(gameObject.lock()->transform->GetLocalPosition(), m_nearBorder);

	if (pocket.lock())
	{
		gameObject.lock()->RemoveGameComponent("Flocking");

		auto stickComponent = gameObject.lock()->AddGameComponent<Stick>();
		stickComponent->SetPocket(pocket);
	}
}
