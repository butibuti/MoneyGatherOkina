#include "stdafx_u.h"
#include "SphereExclusion.h"
#include "ButiBulletWrap/ButiBulletWrap/Common.h"
#include "ButiBulletWrap/ButiBulletWrap/RigidBody.h"

void ButiEngine::SphereExclusion::OnUpdate()
{
}

void ButiEngine::SphereExclusion::OnSet()
{
	gameObject.lock()->AddCollisionStayReaction(std::function<void(ButiBullet::ContactData&)>([](ButiBullet::ContactData& arg_other)->void 
		{

		}));
}

void ButiEngine::SphereExclusion::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::SphereExclusion::Clone()
{
	return ObjectFactory::Create<SphereExclusion>();
}
