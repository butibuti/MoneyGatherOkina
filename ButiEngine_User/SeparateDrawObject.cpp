#include "stdafx_u.h"
#include "SeparateDrawObject.h"

void ButiEngine::SeparateDrawObject::OnUpdate()
{
}

void ButiEngine::SeparateDrawObject::OnSet()
{
}

void ButiEngine::SeparateDrawObject::OnRemove()
{
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
	m_vwp_drawObject = gameObject.lock()->GetGameObjectManager().lock()->AddObjectFromCereal("DrawObject_" + arg_objectName);
	m_vwp_drawObject.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);
	return m_vwp_drawObject;
}

void ButiEngine::SeparateDrawObject::Dead()
{
	if (m_vwp_drawObject.lock())
	{
		m_vwp_drawObject.lock()->SetIsRemove(true);
	}
}
