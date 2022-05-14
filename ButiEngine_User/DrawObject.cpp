#include "stdafx_u.h"
#include "DrawObject.h"

void ButiEngine::DrawObject::OnUpdate()
{
}

void ButiEngine::DrawObject::OnSet()
{
}

void ButiEngine::DrawObject::OnShowUI()
{
}

void ButiEngine::DrawObject::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::DrawObject::Clone()
{
	return ObjectFactory::Create<DrawObject>();
}
