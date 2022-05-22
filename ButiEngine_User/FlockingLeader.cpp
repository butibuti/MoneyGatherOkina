#include "stdafx_u.h"
#include "FlockingLeader.h"
#include "Flocking.h"

void ButiEngine::FlockingLeader::OnUpdate()
{
	m_prevPos = m_pos;
	m_pos = gameObject.lock()->transform->GetWorldPosition();
}

void ButiEngine::FlockingLeader::OnSet()
{
	auto tag = GameObjectTag("Leader");
	gameObject.lock()->SetGameObjectTag(tag);

	Flocking::ResetWorkers();
}

void ButiEngine::FlockingLeader::OnRemove()
{
	gameObject.lock()->RemoveGameObjectTag(GameObjectTag("Leader"));
}

void ButiEngine::FlockingLeader::OnShowUI()
{
}

void ButiEngine::FlockingLeader::Start()
{
	m_pos = gameObject.lock()->transform->GetWorldPosition();
	m_prevPos = m_pos;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FlockingLeader::Clone()
{
	return ObjectFactory::Create<FlockingLeader>();
}
