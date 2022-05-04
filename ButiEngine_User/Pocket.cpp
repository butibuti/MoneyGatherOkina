#include "stdafx_u.h"
#include "Pocket.h"

void ButiEngine::Pocket::OnUpdate()
{
}

void ButiEngine::Pocket::OnSet()
{
}

void ButiEngine::Pocket::OnRemove()
{
	if (m_vwp_worker.lock())
	{
		m_vwp_worker.lock()->SetIsRemove(true);
	}
}

void ButiEngine::Pocket::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Pocket::Clone()
{
	return ObjectFactory::Create<Pocket>();
}
