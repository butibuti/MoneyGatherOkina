#include "stdafx_u.h"
#include "SceneChangeAnimationComponent.h"

void ButiEngine::SceneChangeAnimationComponent::OnUpdate()
{
}

void ButiEngine::SceneChangeAnimationComponent::OnSet()
{
}

void ButiEngine::SceneChangeAnimationComponent::Start()
{
}

void ButiEngine::SceneChangeAnimationComponent::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::SceneChangeAnimationComponent::Clone()
{
	return ObjectFactory::Create<SceneChangeAnimationComponent>();
}
