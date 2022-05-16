#include "stdafx_u.h"
#include "MoveRestriction.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"

void ButiEngine::MoveRestriction::OnUpdate()
{
	//フィールドから出ていたら戻す
	if (IsOutField(gameObject.lock()->transform->GetWorldPosition()))
	{
		SetNewPosition();
	}
}

void ButiEngine::MoveRestriction::OnSet()
{
}

void ButiEngine::MoveRestriction::OnShowUI()
{
}

void ButiEngine::MoveRestriction::Start()
{
	m_vwp_field = GetManager().lock()->GetGameObject(GameObjectTag("Field"));
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::MoveRestriction::Clone()
{
	return ObjectFactory::Create<MoveRestriction>();
}

float ButiEngine::MoveRestriction::GetOutLength(const Vector3& arg_pos)
{
	float radius = gameObject.lock()->transform->GetWorldScale().x * 0.5f;
	float fieldRadius = m_vwp_field.lock()->transform->GetLocalScale().x * 0.5f;

	float border = fieldRadius - radius;

	Vector3 fieldPos = m_vwp_field.lock()->transform->GetLocalPosition();
	fieldPos.y = arg_pos.y;
	float distance = arg_pos.Distance(fieldPos);

	float outLength = max(0.0f, distance - border);

	return outLength;
}

float ButiEngine::MoveRestriction::GetDistanceSqrToFieldCenter(const Vector3& arg_pos)
{
	Vector3 fieldPos = m_vwp_field.lock()->transform->GetLocalPosition();
	fieldPos.y = arg_pos.y;
	float distanceSqr = (arg_pos - fieldPos).GetLengthSqr();
	return distanceSqr;
}

float ButiEngine::MoveRestriction::GetBorderSqr()
{
	float radius = gameObject.lock()->transform->GetWorldScale().x * 0.5f;
	float fieldRadius = m_vwp_field.lock()->transform->GetLocalScale().x * 0.5f;

	float border = fieldRadius - radius;
	float borderSqr = border * border;
	return borderSqr;
}

bool ButiEngine::MoveRestriction::IsOutField(const Vector3& arg_pos)
{
	float radius = gameObject.lock()->transform->GetWorldScale().x * 0.5f;
	float fieldRadius = m_vwp_field.lock()->transform->GetLocalScale().x * 0.5f;

	float border = fieldRadius - radius;
	float borderSqr = border * border;

	Vector3 fieldPos = m_vwp_field.lock()->transform->GetLocalPosition();
	fieldPos.y = arg_pos.y;

	float distanceSqr = (fieldPos - arg_pos).GetLengthSqr();

	return distanceSqr > borderSqr;
}

void ButiEngine::MoveRestriction::SetNewPosition()
{
	float radius = gameObject.lock()->transform->GetWorldScale().x * 0.5f;
	float fieldRadius = m_vwp_field.lock()->transform->GetLocalScale().x * 0.5f;

	float border = fieldRadius - radius;
	float borderSqr = border * border;

	Vector3 pos = gameObject.lock()->transform->GetWorldPosition();
	Vector3 fieldPos = m_vwp_field.lock()->transform->GetLocalPosition();
	fieldPos.y = pos.y;

	Vector3 dir = (pos - fieldPos).GetNormalize();
	Vector3 newPos = fieldPos + dir * border;
	gameObject.lock()->transform->SetWorldPosition(newPos);
}
