#include "stdafx_u.h"
#include "Enemy.h"

void ButiEngine::Enemy::OnUpdate()
{
}

void ButiEngine::Enemy::OnSet()
{
	//gameObject.lock()->AddCollisionStayReaction(std::function<void(ButiBullet::ContactData&)>([](ButiBullet::ContactData& arg_other)->void 
	//	{
	//		GUI::Console("Stay");
	//	}));
}

void ButiEngine::Enemy::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Enemy::Clone()
{
	return ObjectFactory::Create<Enemy>();
}
