#include "stdafx_u.h"
#include "Crystal.h"
#include "Enemy.h"
#include "SeparateDrawObject.h"
#include "SphereExclusion.h"

void ButiEngine::Crystal::OnUpdate()
{
	//ƒ_ƒ[ƒW”»’èŽæ‚èÁ‚µ
	if (m_vlp_removeTagTimer->Update())
	{
		gameObject.lock()->RemoveGameObjectTag(GameObjectTag("PlayerDamageArea"));
		m_vlp_removeTagTimer->Stop();
	}
}

void ButiEngine::Crystal::OnSet()
{
}

void ButiEngine::Crystal::OnRemove()
{
}

void ButiEngine::Crystal::OnShowUI()
{
}

void ButiEngine::Crystal::Start()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->CreateDrawObject("Crystal");

	SetEnemyParameter();

	gameObject.lock()->GetGameComponent<SphereExclusion>()->SetWeight(100.0f);

	m_vlp_removeTagTimer = ObjectFactory::Create<RelativeTimer>(2);
	m_vlp_removeTagTimer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Crystal::Clone()
{
	return ObjectFactory::Create<Crystal>();
}

void ButiEngine::Crystal::SetEnemyParameter()
{
	auto vlp_enemyComponent = gameObject.lock()->GetGameComponent<Enemy>();
	vlp_enemyComponent->CreatePocket(3);
	vlp_enemyComponent->SetVibrationCapacity(1000.0f);
	vlp_enemyComponent->SetVibrationResistance(1.0f);
	vlp_enemyComponent->SetExplosionScale(2.0f);
	vlp_enemyComponent->SetWeight(100.0f);
}
