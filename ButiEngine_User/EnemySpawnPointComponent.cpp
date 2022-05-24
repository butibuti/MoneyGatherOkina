#include "stdafx_u.h"
#include "EnemySpawnPointComponent.h"

void ButiEngine::EnemySpawnPointComponent::OnUpdate()
{
	if (m_vlp_spawnAnimationTimer->Update())
	{
		float randomRotateY = 0;
		randomRotateY = (float)ButiRandom::GetInt(-180, 180);

		Value_weak_ptr<GameObject> enemy;

		//スポーンさせる
		switch (m_spawnType)
		{
		case 0: //ハエ
			enemy = GetManager().lock()->AddObjectFromCereal("Enemy_Fly");
			break;
		case 1: //ストーカー
			enemy = GetManager().lock()->AddObjectFromCereal("Enemy_Stalker");
			break;
		case 2: //キバ
			enemy = GetManager().lock()->AddObjectFromCereal("Enemy_Kiba");
			break;
		case 3: //カザン
			enemy = GetManager().lock()->AddObjectFromCereal("Enemy_Volcano");
			break;
		}

		enemy.lock()->transform->SetLocalPosition(m_position);
		enemy.lock()->transform->SetLocalRotationY_Degrees(randomRotateY);

		m_vlp_diamond.lock()->SetIsRemove(true);
		gameObject.lock()->SetIsRemove(true);
	}
	else
	{
		m_scale += m_moveScale * GameDevice::WorldSpeed;
		m_vlp_diamond.lock()->transform->SetLocalScale(m_scale);
		m_vlp_diamond.lock()->transform->RollLocalRotationY_Degrees(5.0f * GameDevice::WorldSpeed);
	}
}

void ButiEngine::EnemySpawnPointComponent::OnSet()
{
	m_vlp_spawnAnimationTimer = ObjectFactory::Create<RelativeTimer>(120);
	m_position = Vector3(0, 0, 0);
	m_scale = Vector3(0, 0, 0);
	m_moveScale = 0;
	m_spawnType = 0;
}

void ButiEngine::EnemySpawnPointComponent::Start()
{
	m_vlp_spawnAnimationTimer->Start();
	m_vlp_diamond = GetManager().lock()->AddObjectFromCereal("EnemySpawnDiamond");
	m_vlp_diamond.lock()->transform->SetLocalPosition(m_position);

	switch (m_spawnType)
	{
	case 0: //ハエ
		m_moveScale = 1.0f / 120.0f;
		break;
	case 1: //ストーカー
		m_moveScale = 1.0f / 120.0f;
		break;
	case 2: //キバ
		m_moveScale = 4.0f / 120.0f;
		break;
	case 3: //カザン
		m_moveScale = 5.0f / 120.0f;
		break;
	}
}

void ButiEngine::EnemySpawnPointComponent::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::EnemySpawnPointComponent::Clone()
{
	return ObjectFactory::Create<EnemySpawnPointComponent>();
}
