#include "stdafx_u.h"
#include "Pocket.h"
#include "Worker.h"

void ButiEngine::Pocket::OnUpdate()
{
}

void ButiEngine::Pocket::OnSet()
{
}

void ButiEngine::Pocket::OnRemove()
{
}

void ButiEngine::Pocket::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Pocket::Clone()
{
	return ObjectFactory::Create<Pocket>();
}

void ButiEngine::Pocket::Dead()
{
	if (m_vwp_worker.lock())
	{
		auto worker = m_vwp_worker.lock()->GetGameComponent<Worker>();
		if (worker)
		{
			worker->Dead();
		}
	}

	gameObject.lock()->SetIsRemove(true);
}
