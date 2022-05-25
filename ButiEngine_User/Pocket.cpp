#include "stdafx_u.h"
#include "Pocket.h"
#include "Worker.h"
#include "Enemy.h"
#include "Stick.h"

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
			Vector3 enemyPos = m_vwp_enemy.lock()->transform->GetLocalPosition();
			Vector3 workerPos = m_vwp_worker.lock()->transform->GetWorldPosition();
			Vector3 dir = (workerPos - enemyPos).GetNormalize();
			auto stick = m_vwp_worker.lock()->GetGameComponent<Stick>();
			if (stick)
			{
				stick->Dead();
			}
			worker->Rupture(dir);

			m_vwp_enemy.lock()->GetGameComponent<Enemy>()->RemoveSrickWorkerCount();
		}
	}

	gameObject.lock()->SetIsRemove(true);
}
