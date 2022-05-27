#include "stdafx_u.h"
#include "WorkerSpawnEffect.h"

void ButiEngine::WorkerSpawnEffect::OnUpdate()
{


	if (m_lifeTimer->Update())
	{
		gameObject.lock()->transform->SetLocalScale(m_targetScale);
	}
}

void ButiEngine::WorkerSpawnEffect::OnSet()
{
}

void ButiEngine::WorkerSpawnEffect::OnRemove()
{
}

void ButiEngine::WorkerSpawnEffect::OnShowUI()
{
}

void ButiEngine::WorkerSpawnEffect::Start()
{
	m_lifeTimer = ObjectFactory::Create<RelativeTimer>(20);
	m_lifeTimer->Start();
	m_startScale = gameObject.lock()->transform->GetLocalScale();
	m_targetScale = m_startScale;
	m_targetScale.x = 0.0f;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::WorkerSpawnEffect::Clone()
{
	return ObjectFactory::Create<WorkerSpawnEffect>();
}
