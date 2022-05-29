#include "stdafx_u.h"
#include "CameraComponent.h"
#include "InputManager.h"

void ButiEngine::CameraComponent::OnUpdate()
{
#ifdef DEBUG
    InputOperationNum();
#endif // DEBUG

    FixOperationNum();
    ZoomOperation(m_zoomOperationNum);
}

void ButiEngine::CameraComponent::OnSet()
{
}

void ButiEngine::CameraComponent::Start()
{
    //�J�����̏����ʒu
    //m_defaultPosition = Vector3(0, 7.0f, -10.0f);
    m_defaultPosition = Vector3(0.0f, 24.0f, -7.0f);
    m_currentPosition = Vector3(0.0f, 15.0f, -5.0f);
    m_previousPosition = m_currentPosition;

    //�J�����̏����p�x
    //m_defaultRotateX = 0.6f;
    m_defaultRotateX = 1.3f;
    m_currentRotateX = m_defaultRotateX;
    m_previousRotateX = m_currentRotateX;

    //0�`4
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
        CanZoom(Vector3(0.0f, 5.0f, -7.0f), 0.6f);
        break;
    case 1:
        CanZoom(Vector3(0.0f, 15.0f, -5.0f), 1.3f);
        break;
    case 2: //�f�t�H���g
        CanZoom(m_defaultPosition, m_defaultRotateX);
        break;
    case 3: //�Y�[���A�E�g
        //CanZoom(Vector3(0.0f, 24.0f, -16.0f), 1.0f);
        CanZoom(Vector3(0.0f, 50.0f, -15.0f), 1.3f);
        break;
    case 4: //�E���g���Y�[���A�E�g
        //CanZoom(Vector3(0.0f, 36.0f, -10.0f), 1.3f);
        //CanZoom(Vector3(0.0f, 60.0f, -10.0f), 1.3f);
        CanZoom(Vector3(0.0f, 60.0f, 0.0f), 1.5f);
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
    //0�`4�ɏC��
    if (m_zoomOperationNum < 0)
    {
        m_zoomOperationNum = 0;
    }
    else if (m_zoomOperationNum > 4)
    {
        m_zoomOperationNum = 4;
    }
}

void ButiEngine::CameraComponent::CameraReset()
{
    //�J�������f�t�H���g�̈ʒu�ɖ߂�
    m_zoomOperationNum = 2;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::CameraComponent::Clone()
{
	return ObjectFactory::Create<CameraComponent>();
}
