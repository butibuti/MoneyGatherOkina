#include "stdafx_u.h"
#include "Worker.h"

void ButiEngine::Worker::OnUpdate()
{
}

void ButiEngine::Worker::OnSet()
{
	//gameObject.lock()->AddCollisionStayReaction(std::function<void(ButiBullet::ContactData&)>([](ButiBullet::ContactData& arg_other)->void 
	//	{
	//		GUI::Console("Stay");
	//	}));
}

void ButiEngine::Worker::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Worker::Clone()
{
	return ObjectFactory::Create<Worker>();
}
