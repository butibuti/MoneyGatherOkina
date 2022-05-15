#include "stdafx_u.h"
#include "PredictedPoint.h"

void ButiEngine::PredictedPoint::OnUpdate()
{
	if (m_vlp_lifeTimer->Update())
	{
		gameObject.lock()->SetIsRemove(true); //Ž€
	}
}

void ButiEngine::PredictedPoint::OnSet()
{
	m_vlp_lifeTimer = ObjectFactory::Create<RelativeTimer>();
}

void ButiEngine::PredictedPoint::OnShowUI()
{
}

void ButiEngine::PredictedPoint::Start()
{
	m_vlp_lifeTimer->Start();
	m_vlp_lifeTimer->ChangeCountFrame(105);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::PredictedPoint::Clone()
{
	return ObjectFactory::Create<PredictedPoint>();
}
