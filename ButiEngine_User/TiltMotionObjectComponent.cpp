#include "stdafx_u.h"
#include "TiltMotionObjectComponent.h"

void ButiEngine::TiltMotionObjectComponent::OnUpdate()
{
}

void ButiEngine::TiltMotionObjectComponent::OnSet()
{
}

void ButiEngine::TiltMotionObjectComponent::OnShowUI()
{
}

void ButiEngine::TiltMotionObjectComponent::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::TiltMotionObjectComponent::Clone()
{
	return ObjectFactory::Create<TiltMotionObjectComponent>();
}
