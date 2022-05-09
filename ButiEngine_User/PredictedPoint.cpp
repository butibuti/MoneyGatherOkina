#include "stdafx_u.h"
#include "PredictedPoint.h"

void ButiEngine::PredictedPoint::OnUpdate()
{
	if (m_life > 0)
	{
		m_life--;
	}
	else
	{
		gameObject.lock()->SetIsRemove(true); //Ž€
	}
}

void ButiEngine::PredictedPoint::OnSet()
{
}

void ButiEngine::PredictedPoint::OnShowUI()
{
}

void ButiEngine::PredictedPoint::Start()
{
	m_life = 180;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::PredictedPoint::Clone()
{
	return ObjectFactory::Create<PredictedPoint>();
}
