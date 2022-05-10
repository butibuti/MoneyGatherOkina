#include "stdafx_u.h"
#include "Explosion.h"
#include "Suicide.h"

void ButiEngine::Explosion::OnUpdate()
{
}

void ButiEngine::Explosion::OnSet()
{
}

void ButiEngine::Explosion::Start()
{
	gameObject.lock()->AddGameComponent<Suicide>(5);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Explosion::Clone()
{
	return ObjectFactory::Create<Explosion>();
}
