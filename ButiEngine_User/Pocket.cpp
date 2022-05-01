#include "stdafx_u.h"
#include "Pocket.h"

void ButiEngine::Pocket::OnUpdate()
{
}

void ButiEngine::Pocket::OnSet()
{
}

void ButiEngine::Pocket::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Pocket::Clone()
{
	return ObjectFactory::Create<Pocket>();
}
