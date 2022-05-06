#include "stdafx_u.h"
#include "Loiter.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"

void ButiEngine::Loiter::OnUpdate()
{
	if (m_canMove)
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
	m_vlp_rigidBody = gameObject.lock()->GetGameComponent<RigidBodyComponent>();

	m_targetSpawner = ObjectFactory::Create<Transform>();
	m_targetSpawner->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition());

	m_vlp_waitTimer->Start();
	m_vlp_accelTimer->Start();
	m_vlp_brakeTimer->Start();

	m_moveTarget = Vector3Const::Zero;
	m_velocity = Vector3Const::Zero;
	m_moveSpeed = 0.0f;
	m_speedBeforeBrake = 0.0f;
	m_canMove = true;
	m_canAccel = true;
	m_canBrake = false;
	m_isGetSpeedBeforeBrake = false;
	SetMoveTarget();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Loiter::Clone()
{
	return ObjectFactory::Create<Loiter>();
}

void ButiEngine::Loiter::Stop()
{
}

void ButiEngine::Loiter::Move()
{
	if (m_canAccel)
	{
		Accel();
	}

	//ターゲットにある程度近づいたらスピードを落とす
	if (m_canBrake)
	{
		Brake();
	}
	else
	{
		float nearBorder = 3.0f;
		float nearBorderSqr = nearBorder * nearBorder;
		float distanceSqr = (m_moveTarget - gameObject.lock()->transform->GetLocalPosition()).GetLengthSqr();
		if (distanceSqr <= nearBorderSqr)
		{
			m_canAccel = false;
			m_canBrake = true;
		}
	}

	gameObject.lock()->transform->Translate(m_velocity * m_moveSpeed);
	m_vlp_rigidBody->TransformApply();
}

void ButiEngine::Loiter::Accel()
{
	m_moveSpeed = m_maxMoveSpeed * Easing::EaseInCirc(m_vlp_accelTimer->GetPercent());

	if (m_vlp_accelTimer->Update())
	{
		m_moveSpeed = m_maxMoveSpeed * Easing::EaseInCirc(1.0f);
		m_canAccel = false;
	}
}

void ButiEngine::Loiter::Brake()
{
	//ブレーキ前の速度を取得
	if (!m_isGetSpeedBeforeBrake)
	{
		m_speedBeforeBrake = m_moveSpeed;
		m_isGetSpeedBeforeBrake = true;
	}

	float a = Easing::EaseInCirc(m_vlp_brakeTimer->GetPercent());
	m_moveSpeed = m_speedBeforeBrake * (1.0f - Easing::EaseInCirc(m_vlp_brakeTimer->GetPercent()));

	if (m_vlp_brakeTimer->Update())
	{
		m_moveSpeed = m_speedBeforeBrake * Easing::EaseInCirc(1.0f);
		m_canBrake = false;
		m_canMove = false;
	}
}

void ButiEngine::Loiter::Wait()
{
	if (m_vlp_waitTimer->Update())
	{
		SetMoveTarget();

		m_canMove = true;
		m_canAccel = true;
		m_isGetSpeedBeforeBrake = false;
	}
}

void ButiEngine::Loiter::SetMoveTarget()
{
	m_targetSpawner->RollLocalRotationY_Degrees(ButiRandom::GetRandom(110, 250));
	m_moveTarget = m_targetSpawner->GetLocalPosition() + m_targetSpawner->GetFront() * ButiRandom::GetRandom(m_moveRange * 0.75f, m_moveRange);

	m_velocity = (m_moveTarget - gameObject.lock()->transform->GetLocalPosition()).GetNormalize();
}
