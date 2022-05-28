#include "stdafx_u.h"
#include "SpawnEffect.h"
#include "ParticleGenerater.h"

void ButiEngine::SpawnEffect::OnUpdate()
{
	float progress = m_lifeTimer->GetPercent();
	progress = min(progress, 1.0f);

	Vector3 scale = MathHelper::LerpPosition(m_startScale, m_targetScale, Easing::EaseOutExpo(progress));
	gameObject.lock()->transform->SetLocalScale(scale);

	if (m_lifeTimer->Update())
	{
		gameObject.lock()->transform->SetLocalScale(m_targetScale);
		gameObject.lock()->SetIsRemove(true);
	}
}

void ButiEngine::SpawnEffect::OnSet()
{
}

void ButiEngine::SpawnEffect::OnRemove()
{
}

void ButiEngine::SpawnEffect::OnShowUI()
{
}

void ButiEngine::SpawnEffect::Start()
{
	m_vwp_polygonParticleGenerater = GetManager().lock()->GetGameObject("PolygonParticleController").lock()->GetGameComponent<ParticleGenerater>();

	m_lifeTimer = ObjectFactory::Create<RelativeTimer>(20);
	m_lifeTimer->Start();
	m_startScale = gameObject.lock()->transform->GetLocalScale();
	m_targetScale = m_startScale;
	m_targetScale.x = 0.0f;
	m_targetScale.z = 0.0f;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::SpawnEffect::Clone()
{
	return ObjectFactory::Create<SpawnEffect>();
}

void ButiEngine::SpawnEffect::SetColor(const Vector4& arg_color)
{
	auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
	meshDraw->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color = arg_color;
}
