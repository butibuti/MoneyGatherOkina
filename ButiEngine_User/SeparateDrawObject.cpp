#include "stdafx_u.h"
#include "SeparateDrawObject.h"

void ButiEngine::SeparateDrawObject::OnUpdate()
{
	if (!m_vwp_drawObject.lock()) { return; }

	auto transform = gameObject.lock()->transform;
	auto drawObjectTransform = m_vwp_drawObject.lock()->transform;

	drawObjectTransform->SetWorldPosition(transform->GetWorldPosition());
	drawObjectTransform->SetLocalScale(transform->GetLocalScale());
}

void ButiEngine::SeparateDrawObject::OnSet()
{
}

void ButiEngine::SeparateDrawObject::OnRemove()
{
	if (!m_vwp_drawObject.lock()) { return; }
	m_vwp_drawObject.lock()->SetIsRemove(true);
}

void ButiEngine::SeparateDrawObject::OnShowUI()
{
}

void ButiEngine::SeparateDrawObject::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::SeparateDrawObject::Clone()
{
	return ObjectFactory::Create<SeparateDrawObject>();
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::SeparateDrawObject::CreateDrawObject(const std::string& arg_objectName)
{
	if (m_vwp_drawObject.lock()) { return m_vwp_drawObject; }
	m_vwp_drawObject = gameObject.lock()->GetGameObjectManager().lock()->AddObjectFromCereal("DrawObject_" + arg_objectName, gameObject.lock()->transform->Clone());
	return m_vwp_drawObject;
}
