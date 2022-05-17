#include "stdafx_u.h"
#include "NumberComponent.h"

void ButiEngine::NumberComponent::OnUpdate()
{
	FixNumber();

	//”Ô†‚ª•Ï‚í‚Á‚Ä‚¢‚½‚çXV‚·‚é
	if (m_previousNumber != m_number)
	{
		m_previousNumber = m_number;

		auto end = m_vec_vlp_meshDrawComponents.end();
		for (auto itr = m_vec_vlp_meshDrawComponents.begin(); itr != end; ++itr)
		{
			(*itr)->GetTransform()->SetLocalPosition(Vector3(0, 0, 50000));
		}
	}

	m_vec_vlp_meshDrawComponents[m_number]->GetTransform()->SetLocalPosition(Vector3(0, 0, 0));
	m_vec_vlp_meshDrawComponents[m_number]->GetTransform()->SetLocalScale(m_scale);
	m_vec_vlp_meshDrawComponents[m_number]->GetTransform()->SetLocalRotationX_Degrees(m_rotate.x);
	m_vec_vlp_meshDrawComponents[m_number]->GetTransform()->SetLocalRotationY_Degrees(m_rotate.y);
	m_vec_vlp_meshDrawComponents[m_number]->GetTransform()->SetLocalRotationZ_Degrees(m_rotate.z);
}

void ButiEngine::NumberComponent::OnSet()
{
	m_rotate = Vector3(0, 0, 0);
	m_color = Vector4(1, 1, 1, 1);
	m_scale = Vector3(10, 10, 10);

	m_previousNumber = 0;
	m_number = 0;
}

void ButiEngine::NumberComponent::OnShowUI()
{
	GUI::BulletText("Number");
	GUI::InputInt("##number", m_number);
}

void ButiEngine::NumberComponent::Start()
{
	for (std::int8_t i = 0; i < 10; i++)
	{
		auto meshDrawComponent = gameObject.lock()->GetGameComponent<MeshDrawComponent>(i);
		m_vec_vlp_meshDrawComponents.push_back(meshDrawComponent);
		meshDrawComponent->GetTransform()->SetLocalPosition(Vector3(0, 0, 50000));
		meshDrawComponent->GetTransform()->SetLocalScale(m_scale);
		meshDrawComponent->GetTransform()->SetLocalRotationX_Degrees(m_rotate.x);
		meshDrawComponent->GetTransform()->SetLocalRotationY_Degrees(m_rotate.y);
		meshDrawComponent->GetTransform()->SetLocalRotationZ_Degrees(m_rotate.z);
	}
	m_vec_vlp_meshDrawComponents[m_number]->GetTransform()->SetLocalPosition(Vector3(0, 0, 0));
	UpdateColor();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::NumberComponent::Clone()
{
	return ObjectFactory::Create<NumberComponent>();
}

void ButiEngine::NumberComponent::UpdateColor()
{
	auto end = m_vec_vlp_meshDrawComponents.end();
	for (auto itr = m_vec_vlp_meshDrawComponents.begin(); itr != end; ++itr)
	{
		(*itr)->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color = m_color;
		(*itr)->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().lightDir = Vector4(0, 0, 1, 1);
	}
}

void ButiEngine::NumberComponent::FixNumber()
{
	if (m_number < 0)
	{
		m_number = 0;
	}
	else if(m_number > 9)
	{
		m_number = 9;
	}
}
