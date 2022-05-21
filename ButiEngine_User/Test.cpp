#include "stdafx_u.h"
#include "Test.h"

void ButiEngine::TestBehavior::OnUpdate()
{
	auto pos = GetCamera("main")->WorldToScreen(gameObject.lock()->transform->GetWorldPosition());
	pos.z = 0;
	vlp_UIObject->transform->SetLocalPosition(pos);
}



void ButiEngine::TestBehavior::Start()
{
	vlp_UIObject = GetManager().lock()->GetGameObject("UIObject");

}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::TestBehavior::Clone()
{
	auto clone = ObjectFactory::Create<TestBehavior>();
	return clone;
}