#include "stdafx_u.h"
#include "Loiter.h"
#include "MoveRestriction.h"

void ButiEngine::Loiter::OnUpdate()
{
	if (m_isMove)
	{
		Move();
	}
	else
	{
		Wait();
	}
}

void ButiEngine::Loiter::OnSet()
{
	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>();
	m_vlp_accelTimer = ObjectFactory::Create<RelativeTimer>();
	m_vlp_brakeTimer = ObjectFactory::Create<RelativeTimer>();
}

void ButiEngine::Loiter::OnShowUI()
{
	GUI::BulletText("MaxMoveSpeed");
	GUI::DragFloat("##speed", &m_maxMoveSpeed, 0.01f, 0.0f, 10.0f);
	GUI::BulletText("MoveRange");
	GUI::DragFloat("##range", &m_moveRange, 0.1f, 0.0f, 100.0f);
	GUI::BulletText("WaitFrame");
	GUI::DragInt("##waitFrame", m_waitFrame, 1, 0, 20);
	if (GUI::Button("SetWait"))
	{
		m_vlp_waitTimer->ChangeCountFrame(m_waitFrame);
	}
	GUI::BulletText("AccelFrame");
	GUI::DragInt("##accelFrame", m_accelFrame, 1, 0, 20);
	if (GUI::Button("SetAccel"))
	{
		m_vlp_accelTimer->ChangeCountFrame(m_accelFrame);
	}
	GUI::BulletText("BrakeFrame");
	GUI::DragInt("##brakeFrame", m_brakeFrame, 1, 0, 20);
	if (GUI::Button("SetBrake"))
	{
		m_vlp_brakeTimer->ChangeCountFrame(m_brakeFrame);
	}
}

void ButiEngine::Loiter::Start()
{
	m_targetSpawner = ObjectFactory::Create<Transform>();
	m_targetSpawner->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition());

	m_vlp_waitTimer->Start();
	m_vlp_accelTimer->Start();
	m_vlp_brakeTimer->Start();

	m_velocity = Vector3Const::Zero;
	m_moveTarget = Vector3Const::Zero;
	m_moveSpeed = 0.0f;
	m_speedBeforeBrake = 0.0f;
	m_isStop = false;
	m_isMove = true;
	m_isAccel = true;
	m_isBrake = false;
	SetMoveTarget();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Loiter::Clone()
{
	return ObjectFactory::Create<Loiter>();
}

void ButiEngine::Loiter::StartBrake()
{
	if (m_isBrake) { return; }

	m_vlp_accelTimer->Reset();
	m_isAccel = false;

	m_speedBeforeBrake = m_moveSpeed;
	m_isBrake = true;
}

void ButiEngine::Loiter::MoveStart()
{
	m_isStop = false;
}

void ButiEngine::Loiter::MoveStop()
{
	if (m_isStop) { return; }
	m_vlp_waitTimer->Reset();
	StartBrake();
	m_isStop = true;
}

void ButiEngine::Loiter::Move()
{
	if (m_isAccel)
	{
		Accel();
	}

	//ターゲットにある程度近づいたらスピードを落とす
	if (m_isBrake)
	{
		Brake();
	}
	else
	{
		float nearBorder = m_moveRange * 0.3f;
		float nearBorderSqr = nearBorder * nearBorder;
		float distanceSqr = (m_moveTarget - gameObject.lock()->transform->GetLocalPosition()).GetLengthSqr();
		if (distanceSqr <= nearBorderSqr)
		{
			StartBrake();
		}
	}

	gameObject.lock()->transform->Translate(m_velocity * m_moveSpeed);
}

void ButiEngine::Loiter::Accel()
{
	m_moveSpeed = m_maxMoveSpeed * Easing::EaseInCubic(m_vlp_accelTimer->GetPercent());

	if (m_vlp_accelTimer->Update())
	{
		m_moveSpeed = m_maxMoveSpeed;
		m_isAccel = false;
	}
}

void ButiEngine::Loiter::Brake()
{
	m_moveSpeed = m_speedBeforeBrake * (1.0f - Easing::EaseInCubic(m_vlp_brakeTimer->GetPercent()));

	if (m_vlp_brakeTimer->Update())
	{
		m_moveSpeed = 0.0f;
		m_isBrake = false;
		m_isMove = false;
	}
}

void ButiEngine::Loiter::Wait()
{
	if (m_isStop) { return; }

	if (m_vlp_waitTimer->Update())
	{
		SetMoveTarget();

		m_isMove = true;
		m_isAccel = true;
	}
}

void ButiEngine::Loiter::SetMoveTarget()
{
	m_targetSpawner->RollLocalRotationY_Degrees(ButiRandom::GetRandom(110, 250));
	m_moveTarget = m_targetSpawner->GetLocalPosition() + m_targetSpawner->GetFront() * ButiRandom::GetRandom(m_moveRange * 0.9f, m_moveRange);

	//フィールドから出ている分戻す
	auto moveRestriction = gameObject.lock()->GetGameComponent<MoveRestriction>();
	if (moveRestriction->IsOutField(m_moveTarget))
	{
		float outLength = moveRestriction->GetOutLength(m_moveTarget);
		m_moveTarget -= m_targetSpawner->GetFront() * outLength;
	}

	m_velocity = (m_moveTarget - gameObject.lock()->transform->GetLocalPosition()).GetNormalize();
}
