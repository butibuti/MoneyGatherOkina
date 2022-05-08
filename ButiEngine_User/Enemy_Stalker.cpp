#include "stdafx_u.h"
#include "Enemy_Stalker.h"

void ButiEngine::Enemy_Stalker::OnUpdate()
{
}

void ButiEngine::Enemy_Stalker::OnSet()
{
}

void ButiEngine::Enemy_Stalker::OnRemove()
{
}

void ButiEngine::Enemy_Stalker::OnShowUI()
{
}

void ButiEngine::Enemy_Stalker::Start()
{
	SetEnemyParameter();
	SetLoiterParameter();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Enemy_Stalker::Clone()
{
	return ObjectFactory::Create<Enemy_Stalker>();
}

void ButiEngine::Enemy_Stalker::SetEnemyParameter()
{
}

void ButiEngine::Enemy_Stalker::SetLoiterParameter()
{
}
