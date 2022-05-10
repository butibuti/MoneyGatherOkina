#include "stdafx_u.h"
#include "Suicide.h"

void ButiEngine::Suicide::OnUpdate()
{
	if (m_vlp_timer->Update())
	{
		gameObject.lock()->SetIsRemove(true);
	}
}

void ButiEngine::Suicide::OnSet()
{
	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(m_suicideFrame);
	m_vlp_timer->Start();
}

void ButiEngine::Suicide::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Suicide::Clone()
{
	return ObjectFactory::Create<Suicide>();
}
