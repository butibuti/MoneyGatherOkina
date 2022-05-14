#include "stdafx_u.h"
#include "FloatMotionParent.h"

void ButiEngine::FloatMotionParent::OnUpdate()
{
}

void ButiEngine::FloatMotionParent::OnSet()
{
}

void ButiEngine::FloatMotionParent::OnShowUI()
{
}

void ButiEngine::FloatMotionParent::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FloatMotionParent::Clone()
{
	return ObjectFactory::Create<FloatMotionParent>();
}
