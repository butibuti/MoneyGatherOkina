#include "stdafx_u.h"
#include "Stick.h"

void ButiEngine::Stick::OnUpdate()
{
}

void ButiEngine::Stick::OnSet()
{
}

void ButiEngine::Stick::OnShowUI()
{
}

void ButiEngine::Stick::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Stick::Clone()
{
	return ObjectFactory::Create<Stick>();
}
