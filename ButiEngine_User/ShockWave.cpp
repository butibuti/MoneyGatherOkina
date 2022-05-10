#include "stdafx_u.h"
#include "ShockWave.h"

void ButiEngine::ShockWave::OnUpdate()
{
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
	m_vwp_player = GetManager().lock()->GetGameObject(GameObjectTag("Player"));

	m_maxScale = Vector3(10.0f, 10.0f, 10.0f);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::ShockWave::Clone()
{
	return ObjectFactory::Create<ShockWave>();
}

void ButiEngine::ShockWave::Appear()
{
	Vector3 playerScale = m_vwp_player.lock()->transform->GetLocalScale();
	Vector3 scale = Vector3(10.0f, 10.0f, 10.0f) / playerScale;
	gameObject.lock()->transform->SetLocalScale(scale);
}

void ButiEngine::ShockWave::Disappear()
{
	gameObject.lock()->transform->SetLocalScale(0.0f);
}
