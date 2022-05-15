#include "stdafx_u.h"
#include "PolygonParticleGenerator.h"
#include "InputManager.h"

void ButiEngine::PolygonParticleGenerater::OnUpdate()
{
	Flickering();
}

void ButiEngine::PolygonParticleGenerater::OnSet()
{
}

void ButiEngine::PolygonParticleGenerater::OnShowUI()
{
}

void ButiEngine::PolygonParticleGenerater::Start()
{
	if (m_vwp_immediateParticleController.lock() == nullptr)
	{
		m_vwp_immediateParticleController = gameObject.lock()->GetGameComponent<ImmediateParticleController>();
	}
	m_flickeringFrame = 0;
	m_isChangeColor = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::PolygonParticleGenerater::Clone()
{
	return ObjectFactory::Create<PolygonParticleGenerater>();
}

void ButiEngine::PolygonParticleGenerater::Flickering()
{
	if (m_flickeringFrame < 2)
	{
		m_flickeringFrame++;
	}
	else
	{
		m_flickeringFrame = 0;
		m_isChangeColor = !m_isChangeColor;
	}

	//�����Ŗ��ł������p�[�e�B�N���̎Q�Ƃ����A���ł���悤�ɍX�V��������(�H)
	auto& particleList = m_vwp_immediateParticleController.lock()->GetParticles();

	for (auto itr = particleList.begin(); itr != particleList.end(); ++itr)
	{
		if ((*itr).velocity != Vector3(0, 0, 0))
		{
			//���ł��������Ȃ��p�[�e�B�N���͖���
			continue;
		}

		if (!m_isChangeColor)
		{
			(*itr).color = Vector4(1, 1, 1, 1);
		}
		else
		{
			(*itr).color = ButiColor::Yellow(ButiColor::ShadeIndex::Shade_9);
		}
	}
}
