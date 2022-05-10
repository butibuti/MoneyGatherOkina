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
}

void ButiEngine::Suicide::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Suicide::Clone()
{
	return ObjectFactory::Create<Suicide>();
}
