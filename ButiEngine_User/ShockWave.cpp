#include "stdafx_u.h"
#include "ShockWave.h"

void ButiEngine::ShockWave::OnUpdate()
{
	gameObject.lock()->transform->SetLocalScale(m_calcScale);
}

void ButiEngine::ShockWave::OnSet()
{
}

void ButiEngine::ShockWave::OnShowUI()
{
	GUI::BulletText("MaxScale");
	GUI::DragFloat3("##scale", &m_maxScale.x, 0.1f, 0.0f, 100.0f);
}

void ButiEngine::ShockWave::Start()
{
	m_calcScale = Vector3(0, 0, 0);
	m_maxScale = Vector3(9.0f, 9.0f, 9.0f);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::ShockWave::Clone()
{
	return ObjectFactory::Create<ShockWave>();
}

void ButiEngine::ShockWave::Disappear()
{
	m_calcScale = Vector3(0, 0, 0);
}

void ButiEngine::ShockWave::SetScale(const float arg_vibrationPower)
{
	float vibPower = arg_vibrationPower;
	float scaleRate = 0.25f + vibPower;
	m_calcScale = m_maxScale * scaleRate;
}
