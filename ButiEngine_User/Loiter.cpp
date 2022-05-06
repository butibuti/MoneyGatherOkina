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
	m_vlp_timer = ObjectFactory::Create<RelativeTimer>();
}

void ButiEngine::Loiter::OnShowUI()
{
	GUI::BulletText("MaxMoveSpeed");
	GUI::DragFloat("##speed", &m_maxMoveSpeed, 0.01f, 0.0f, 10.0f);
	GUI::BulletText("MoveRange");
	GUI::DragFloat("##range", &m_moveRange, 0.1f, 0.0f, 100.0f);
	GUI::BulletText("WaitFrame");
	GUI::DragInt("##p", m_testWaitFrame, 1, 0, 20);
	if (GUI::Button("Set"))
	{
		m_vlp_timer->ChangeCountFrame(m_testWaitFrame);
	}
}

void ButiEngine::Loiter::Start()
{
	m_vlp_rigidBody = gameObject.lock()->GetGameComponent<RigidBodyComponent>();

	m_targetSpawner = ObjectFactory::Create<Transform>();
	m_targetSpawner->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition());

	m_canMove = true;
	SetMoveTarget();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Loiter::Clone()
{
	return ObjectFactory::Create<Loiter>();
}

void ButiEngine::Loiter::Move()
{
	Vector3 velocity = (m_moveTarget - gameObject.lock()->transform->GetLocalPosition()).GetNormalize();

	//�^�[�Q�b�g�ɋ߂Â�����X�s�[�h�𗎂Ƃ�
	float nearBorder = m_moveRange * 0.5f;
	float nearBorderSqr = nearBorder * nearBorder;
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	float distanceSqr = (pos - m_moveTarget).GetLengthSqr();
	float progress = min(distanceSqr / m_moveRange, 1.0f);
	float moveSpeed = m_maxMoveSpeed * Easing::Parabola(progress);

	gameObject.lock()->transform->Translate(velocity * moveSpeed);
	m_vlp_rigidBody->TransformApply();

	//�X�s�[�h������̈ꊄ�ȉ��ɂȂ�����~�܂�
	float stopBorder = m_maxMoveSpeed * 0.1f;
	if (moveSpeed <= stopBorder)
	{
		m_vlp_timer->Start();
		m_canMove = false;
	}
}

void ButiEngine::Loiter::Wait()
{
	if (m_vlp_timer->Update())
	{
		//�^�C�}�[���Z�b�g
		m_vlp_timer->Stop();
		m_vlp_timer->Reset();

		//�V�����^�[�Q�b�g�w��
		SetMoveTarget();

		m_canMove = true;
	}
}

void ButiEngine::Loiter::SetMoveTarget()
{
	m_targetSpawner->RollLocalRotationY_Degrees(ButiRandom::GetRandom(110, 250));
	m_moveTarget = m_targetSpawner->GetLocalPosition() + m_targetSpawner->GetFront() * ButiRandom::GetRandom(m_moveRange * 0.75f, m_moveRange);
}
