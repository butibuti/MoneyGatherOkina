#include "stdafx_u.h"
#include "TiltMotion.h"
#include "InputManager.h"
#include "DrawObject.h"

void ButiEngine::TiltMotion::OnUpdate()
{
	m_front = m_vwp_parent.lock()->transform->GetFront();
	m_front.y = 0.0f;
	m_front.Normalize();

	m_rotationTarget = (m_vlp_lookTarget->GetLocalPosition() - m_vwp_parent.lock()->transform->GetLocalPosition()).GetNormalize();
	m_rotationTarget.y = 0.0f;

	float dot = m_front.Dot(m_rotationTarget);
	dot = min(dot, 1.0f);

	float angle = MathHelper::ToDegree(std::acos(dot));
	
	float progress = angle / 180.0f;
	float rotationDir = CalculateRotationDirection();
	float targetAngle = MathHelper::Lerp(0.0f, 90.0f, progress) * rotationDir;
	float rotationAngle = gameObject.lock()->transform->GetLocalRotation_Euler().z;

	gameObject.lock()->transform->SetLocalRotationZ_Degrees(MathHelper::Lerp(rotationAngle, targetAngle, m_motionSpeed * GameDevice::WorldSpeed));
}

void ButiEngine::TiltMotion::OnSet()
{
}

void ButiEngine::TiltMotion::OnShowUI()
{
	GUI::BulletText("MotionSpeed");
	GUI::DragFloat("##speed", &m_motionSpeed, 0.01f, 0.0f, 1.0f);
}

void ButiEngine::TiltMotion::Start()
{
	m_vlp_lookTarget = m_vwp_parent.lock()->GetGameComponent<LookAtComponent>()->GetLookTarget();
	m_front = m_vwp_parent.lock()->transform->GetFront();
	m_rotationTarget = (m_vlp_lookTarget->GetLocalPosition() - m_vwp_parent.lock()->transform->GetLocalPosition()).GetNormalize();
	m_motionSpeed = 0.1f;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::TiltMotion::Clone()
{
	return ObjectFactory::Create<TiltMotion>();
}

float ButiEngine::TiltMotion::CalculateRotationDirection()
{
	float rotationDir = 1.0f;
	float cross = m_front.x * m_rotationTarget.z - m_front.z * m_rotationTarget.x;
	if (cross <= 0.0f)
	{
		rotationDir = -1.0f;
	}

	return rotationDir;
}
