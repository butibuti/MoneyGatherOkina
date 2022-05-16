#include "stdafx_u.h"
#include "SceneChangeAnimationComponent.h"
#include "HexagonAnimationComponent.h"

void ButiEngine::SceneChangeAnimationComponent::OnUpdate()
{
}

void ButiEngine::SceneChangeAnimationComponent::OnSet()
{
}

void ButiEngine::SceneChangeAnimationComponent::Start()
{
	m_defaultPosition = Vector3(-750, 500, -200.0f);
	m_space = Vector3(433, 375, 0);
	CreateHexagonPanel();
}

void ButiEngine::SceneChangeAnimationComponent::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::SceneChangeAnimationComponent::Clone()
{
	return ObjectFactory::Create<SceneChangeAnimationComponent>();
}

void ButiEngine::SceneChangeAnimationComponent::SceneStart()
{
	if (m_vec_vwp_hexagons[m_panelCount - 1].lock()->GetGameComponent<HexagonAnimationComponent>()->IsAnimation()) { return; }
	auto end = m_vec_vwp_hexagons.end();
	for (auto itr = m_vec_vwp_hexagons.begin(); itr != end; ++itr)
	{
		(*itr).lock()->GetGameComponent<HexagonAnimationComponent>()->StartAnimation(false);
	}
}

void ButiEngine::SceneChangeAnimationComponent::SceneEnd()
{
	if (m_vec_vwp_hexagons[m_panelCount - 1].lock()->GetGameComponent<HexagonAnimationComponent>()->IsAnimation()) { return; }
	auto end = m_vec_vwp_hexagons.end();
	for (auto itr = m_vec_vwp_hexagons.begin(); itr != end; ++itr)
	{
		(*itr).lock()->GetGameComponent<HexagonAnimationComponent>()->StartAnimation(true);
	}
}

bool ButiEngine::SceneChangeAnimationComponent::IsAnimation()
{
	return m_vec_vwp_hexagons[m_panelCount - 1].lock()->GetGameComponent<HexagonAnimationComponent>()->IsAnimation();
}

void ButiEngine::SceneChangeAnimationComponent::CreateHexagonPanel()
{
	//4Å~5É}ÉX
	std::int8_t maxPanelX = 5;
	std::int8_t maxPanelY = 4;
	m_panelCount = maxPanelX * maxPanelY;
	std::int8_t panelNum = 0;

	for (std::int8_t y = 0; y < maxPanelY; y++)
	{
		for (std::int8_t x = 0; x < maxPanelX; x++)
		{
			Vector3 position;
			if (y % 2)
			{
				//äÔêîî‘ñ⁄ÇÕXÇÃà íuÇîºï™Ç∏ÇÁÇ∑
				position = m_defaultPosition + Vector3(m_space.x * x - m_space.x * 0.5f, -m_space.y * y, 0);
			}
			else
			{
				position = m_defaultPosition + Vector3(m_space.x * x, -m_space.y * y, 0);
			}

			auto drawObject = GetManager().lock()->AddObjectFromCereal("DrawObject_Hexagon");
			m_vec_vwp_hexagons.push_back(drawObject);
			drawObject.lock()->transform->SetLocalPosition(position);		
			drawObject.lock()->GetGameComponent<HexagonAnimationComponent>()->SetNumber(panelNum);
			panelNum++;
		}
	}
}
