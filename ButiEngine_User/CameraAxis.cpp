#include "stdafx_u.h"
#include "CameraAxis.h"
#include "InputManager.h"

void ButiEngine::CameraAxis::OnUpdate()
{
	ChasePlayer(0.1f);
	RotateAxis(0.1f);
}

void ButiEngine::CameraAxis::OnSet()
{
}

void ButiEngine::CameraAxis::OnShowUI()
{
	GUI::BulletText("CurrentRotation");
	GUI::DragFloat3("##currentRotation", m_currentRotation, 0.01f, -180.0f, 180.0f);
}

void ButiEngine::CameraAxis::Start()
{
	if (m_vwp_player.lock() == nullptr)
	{
		//�v���C���[�̎擾
		m_vwp_player = GetManager().lock()->GetGameObject("Player");
	}

	m_currentPosition = m_vwp_player.lock()->transform->GetLocalPosition();
	m_previousPosition = m_currentPosition;

	m_currentRotation = Vector3(0, 0, 0);
	m_previousRotation = m_currentRotation;
}

void ButiEngine::CameraAxis::ChasePlayer(const float arg_lerpScale)
{
	//�v���C���[��Ǐ]����
	m_currentPosition = m_vwp_player.lock()->transform->GetLocalPosition();

	//�Ǐ]�̕��(�J�����̓����Ƀf�B���C��������)
	m_previousPosition.x = m_previousPosition.x * (1.0f - arg_lerpScale) + m_currentPosition.x * arg_lerpScale;
	m_previousPosition.y = m_previousPosition.y * (1.0f - arg_lerpScale) + m_currentPosition.y * arg_lerpScale;
	m_previousPosition.z = m_previousPosition.z * (1.0f - arg_lerpScale) + m_currentPosition.z * arg_lerpScale;

	//�f�B���C�������ʂ��Z�b�g����
	gameObject.lock()->transform->SetLocalPosition(m_previousPosition);
}

void ButiEngine::CameraAxis::RotateAxis(const float arg_lerpScale)
{
	//���͂ɉ����ăJ�����̎�����]������
	m_currentRotation.y += InputManager::GetRightStick().x;

	//�Ǐ]�̕��(�J�����̓����Ƀf�B���C��������)
	m_previousRotation.x = m_previousRotation.x * (1.0f - arg_lerpScale) + m_currentRotation.x * arg_lerpScale;
	m_previousRotation.y = m_previousRotation.y * (1.0f - arg_lerpScale) + m_currentRotation.y * arg_lerpScale;
	m_previousRotation.z = m_previousRotation.z * (1.0f - arg_lerpScale) + m_currentRotation.z * arg_lerpScale;
	
	//�f�B���C�������ʂ��Z�b�g����
	gameObject.lock()->transform->SetLocalRotation(m_previousRotation);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::CameraAxis::Clone()
{
	return ObjectFactory::Create<CameraAxis>();
}