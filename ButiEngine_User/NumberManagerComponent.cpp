#include "stdafx_u.h"
#include "NumberManagerComponent.h"
#include "NumberComponent.h"

void ButiEngine::NumberManagerComponent::OnUpdate()
{
	FixNumber();
	if (m_previousNumber != m_number)
	{
		m_previousNumber = m_number;

		UpdateNumbers();
	}
	UpdateParams();
	ScaleAnimation();
}

void ButiEngine::NumberManagerComponent::OnSet()
{
	m_scale = Vector3(14, 14, 14);
	m_rotate = Vector3(0, 0, 0);
	m_color = Vector4(1, 1, 1, 1);
	m_space = -10;
	m_number = 0;
	m_previousNumber = 0;
	m_maxDigit = 1;
}

void ButiEngine::NumberManagerComponent::OnShowUI()
{
}

void ButiEngine::NumberManagerComponent::Start()
{
	for (std::int32_t i = 0; i < m_maxDigit; i++)
	{
		Vector3 position = Vector3((m_scale.x * 10.0f + m_space) * i, 0, 0);
		auto drawNumber = GetManager().lock()->AddObjectFromCereal("DrawObject_Number");
		drawNumber.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
		m_vec_vlp_numbers.push_back(drawNumber);
		drawNumber.lock()->transform->SetLocalPosition(position);
		drawNumber.lock()->GetGameComponent<NumberComponent>()->SetScale(m_scale);
	}

	m_defaultScale = gameObject.lock()->transform->GetLocalScale();
	m_currentScale = m_defaultScale;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::NumberManagerComponent::Clone()
{
	return ObjectFactory::Create<NumberManagerComponent>();
}

void ButiEngine::NumberManagerComponent::SetScaleAnimationActive(const bool arg_isScaleAnimationActive)
{
	m_isScaleAnimationActive = arg_isScaleAnimationActive;
}

void ButiEngine::NumberManagerComponent::ScaleAnimationStart(const Vector3& arg_scale)
{
	m_isScaleAnimationActive = true;
	m_currentScale = arg_scale;
}

void ButiEngine::NumberManagerComponent::FixNumber()
{
	if (GetDigit(m_number) > m_maxDigit)
	{
		std::int32_t calcNum = 1;
		for (std::int8_t i = 0; i < m_maxDigit; i++)
		{
			calcNum *= 10;
		}
		m_number = calcNum - 1;
	}
	else if (m_number < 0)
	{
		m_number = 0;
	}
}

void ButiEngine::NumberManagerComponent::UpdateNumbers()
{
	int calcDigit = m_maxDigit - 1;
	int multiplier = pow(10, calcDigit);
	int calcNumber = m_number;

	for (int i = 0; i < m_maxDigit; ++i)
	{
		Vector3 position = Vector3((m_scale.x * 10.0f + m_space) * i, 0, 0);
		int setNum = calcNumber / multiplier;
		m_vec_vlp_numbers[i]->GetGameComponent<NumberComponent>()->SetNumber(setNum);
		m_vec_vlp_numbers[i]->transform->SetLocalPosition(position);
		calcNumber = calcNumber % multiplier;
		multiplier /= 10;
	}
}

void ButiEngine::NumberManagerComponent::UpdateParams()
{
	auto end = m_vec_vlp_numbers.end();
	for(auto itr = m_vec_vlp_numbers.begin(); itr != end; ++itr)
	{
		auto numberComponent = (*itr)->GetGameComponent<NumberComponent>();
		numberComponent->SetColor(m_color);
		numberComponent->SetScale(m_scale);
		numberComponent->SetRotate(m_rotate);
	}
}

void ButiEngine::NumberManagerComponent::ScaleAnimation()
{
	if (!m_isScaleAnimationActive) { return; }

	m_currentScale.x = MathHelper::Lerp(m_currentScale.x, m_defaultScale.x, 0.2f);
	m_currentScale.y = MathHelper::Lerp(m_currentScale.y, m_defaultScale.y, 0.2f);
	m_currentScale.z = MathHelper::Lerp(m_currentScale.z, m_defaultScale.z, 0.2f);
	gameObject.lock()->transform->SetLocalScale(m_currentScale);
}

//“n‚³‚ê‚½’l‚ÌŒ…”‚ğ•Ô‚·ŠÖ”
std::int32_t ButiEngine::NumberManagerComponent::GetDigit(const std::int32_t arg_number)
{
	std::int32_t _digit = 0;
	std::int32_t number = arg_number;
	if (number == 0) { _digit = 1; }
	while (number != 0) 
	{
		number /= 10;
		_digit++;
	}
	return _digit;
}