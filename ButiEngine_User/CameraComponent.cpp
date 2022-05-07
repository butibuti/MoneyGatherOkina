#include "stdafx_u.h"
#include "CameraComponent.h"
#include "InputManager.h"

void ButiEngine::CameraComponent::OnUpdate()
{
    InputOperationNum();
    FixOperationNum();
    ZoomOperation(m_zoomOperationNum);
}

void ButiEngine::CameraComponent::OnSet()
{
}

void ButiEngine::CameraComponent::Start()
{
    //�J�����̏����ʒu
    m_defaultPosition = Vector3(0, 8.0f, -6.0f);
    m_currentPosition = m_defaultPosition;
    m_previousPosition = m_currentPosition;

    //�J�����̏����p�x
    m_defaultRotateX = 1.0f;
    m_currentRotateX = m_defaultRotateX;
    m_previousRotateX = m_currentRotateX;

    //0�`3
    m_zoomOperationNum = 1;

    if (m_vwp_cameraAxis.lock() == nullptr)
    {
        //�J������]���p�̐e�I�u�W�F�N�g�̎擾
        m_vwp_cameraAxis = GetManager().lock()->GetGameObject("CameraAxis");

        //cameraAxis��e�Ƃ��ăZ�b�g
        gameObject.lock()->transform->SetBaseTransform(m_vwp_cameraAxis.lock()->transform);
    }

}

void ButiEngine::CameraComponent::OnShowUI()
{
    GUI::BulletText("PreviousPosition");
    GUI::DragFloat3("##previousPosition", m_previousPosition, 0.01f, -30.0f, 30.0f);
    GUI::BulletText("RotateX");
    GUI::DragFloat("##rotateX", m_previousRotateX, 0.01f, 0.0f, 2.0f);
}

void ButiEngine::CameraComponent::CanZoom(const Vector3& arg_zoomPosition, const float arg_rotationX)
{
    float lerpScale = 0.1f;

    //position�̕��
    m_currentPosition = arg_zoomPosition;
    m_previousPosition.x = m_previousPosition.x * (1.0f - lerpScale) + m_currentPosition.x * lerpScale;
    m_previousPosition.y = m_previousPosition.y * (1.0f - lerpScale) + m_currentPosition.y * lerpScale;
    m_previousPosition.z = m_previousPosition.z * (1.0f - lerpScale) + m_currentPosition.z * lerpScale;

    //rotationX�̕��
    m_currentRotateX = arg_rotationX;
    m_previousRotateX = m_previousRotateX * (1.0f - lerpScale) + m_currentRotateX * lerpScale;
    
    //��Ԃ����l�̃Z�b�g
    auto transform = gameObject.lock()->transform;
    transform->SetLocalPosition(m_previousPosition);
    transform->SetLocalRotationX(m_previousRotateX);

}

void ButiEngine::CameraComponent::ZoomOperation(const std::int8_t arg_zoomOperationNum)
{
    switch (arg_zoomOperationNum)
    {
    case 0: //�Y�[���C��
        CanZoom(Vector3(0.0f, 4.0f, -2.5f), 0.85f);
        break;
    case 1: //�f�t�H���g
        CanZoom(m_defaultPosition, m_defaultRotateX); //(0,8,-6),1.0f
        break;
    case 2: //�Y�[���A�E�g
        CanZoom(Vector3(0.0f, 12.0f, -8.0f), 1.1f);
        break;
    case 3: //�E���g���Y�[���A�E�g
        CanZoom(Vector3(0.0f, 18.0f, -6.0f), 1.25f);
        break;
    default:
        CanZoom(m_defaultPosition, m_defaultRotateX);
        break;
    }
}

void ButiEngine::CameraComponent::InputOperationNum()
{
    if (InputManager::IsZoomInKey())
    {
        //�Y�[���C��
        m_zoomOperationNum--;
    }
    else if (InputManager::IsZoomOutKey())
    {
        //�Y�[���A�E�g
        m_zoomOperationNum++;
    }
    if (InputManager::IsCameraResetKey())
    {
        //�f�t�H���g�Y�[��
        CameraReset();
    }
}

void ButiEngine::CameraComponent::FixOperationNum()
{
    //0�`3�ɏC��
    if (m_zoomOperationNum < 0)
    {
        m_zoomOperationNum = 0;
    }
    else if (m_zoomOperationNum > 3)
    {
        m_zoomOperationNum = 3;
    }
}

void ButiEngine::CameraComponent::CameraReset()
{
    //�J�������f�t�H���g�̈ʒu�ɖ߂�
    m_zoomOperationNum = 1;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::CameraComponent::Clone()
{
	return ObjectFactory::Create<CameraComponent>();
}
