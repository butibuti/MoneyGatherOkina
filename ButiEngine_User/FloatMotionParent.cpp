#include "stdafx_u.h"
#include "FloatMotionParent.h"

void ButiEngine::FloatMotionParent::OnUpdate()
{
    if (m_moveChangeCount < 60)
    {
        m_moveChangeCount++;
    }
    else
    {
        m_moveChangeCount = 0;
        m_moveChangeFlag = !m_moveChangeFlag;
    }

    if (m_moveChangeFlag)
    {
        m_currentPosition = Vector3(0, -1, 0);
    }
    else
    {
        m_currentPosition = Vector3(0, 1, 0);
    }


    float lerpScale = 0.02f;

    //position‚Ì•âŠÔ
    m_previousPosition.y = m_previousPosition.y * (1.0f - lerpScale) + m_currentPosition.y * lerpScale;


    gameObject.lock()->transform->SetLocalPosition(m_previousPosition);

}

void ButiEngine::FloatMotionParent::OnSet()
{
}

void ButiEngine::FloatMotionParent::OnShowUI()
{
}

void ButiEngine::FloatMotionParent::Start()
{
	m_currentPosition = Vector3(0, 0, 0);
	m_previousPosition = m_currentPosition;
    m_moveChangeCount = 0;
    m_moveChangeFlag = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FloatMotionParent::Clone()
{
	return ObjectFactory::Create<FloatMotionParent>();
}
