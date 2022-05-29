#include "stdafx_u.h"
#include "EnemySpawnPointComponent.h"
#include"SeparateDrawObject.h"
#include"Enemy.h"
constexpr float sec2Div = 1.0f / 120.0f;
void ButiEngine::EnemySpawnPointComponent::OnUpdate()
{
	if (m_vwp_enemyObject.lock()&&!m_list_drawObjectDrawInformation.GetSize()) {
		m_vwp_enemyDrawObject = m_vwp_enemyObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock();
		auto drawcomponents =m_vwp_enemyDrawObject.lock() ->GetGameComponents<MeshDrawComponent>();
		for (auto& drawComponent : drawcomponents) {
			m_list_drawObjectDrawInformation.Add(drawComponent->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation"));
		}
		m_vwp_appearnceObject.lock()->SetIsRemove(true);
	}

	if (m_vlp_brightOffTimer->Update()) {
		gameObject.lock()->SetIsRemove(true);
		if(m_vwp_enemyObject.lock())
		m_vwp_enemyObject.lock()->GetGameComponent<Enemy>()->SetAppearanceEffect(Value_weak_ptr<GameObject>());
	}
	else if(m_vwp_enemyObject.lock()) {
		for (auto& info : m_list_drawObjectDrawInformation) {
			info.lock()->Get().ExInfo.x=Easing::EaseIn( 1.0- m_vlp_brightOffTimer->GetPercent());
		}
	}

	if (m_vlp_spawnAnimationTimer->Update())
	{
		m_vlp_spawnAnimationTimer->Stop();
		
		//スポーンさせる
		switch (m_spawnType)
		{
		case 0: //ハエ
			m_vwp_enemyObject = GetManager().lock()->AddObjectFromCereal("Enemy_Fly");
			break;
		case 1: //ストーカー
			m_vwp_enemyObject = GetManager().lock()->AddObjectFromCereal("Enemy_Stalker");
			break;
		case 2: //キバ
			m_vwp_enemyObject = GetManager().lock()->AddObjectFromCereal("Enemy_Kiba");
			break;
		case 3: //カザン
			m_vwp_enemyObject = GetManager().lock()->AddObjectFromCereal("Enemy_Volcano");
			break;
		}

		m_vwp_enemyObject.lock()->transform->SetLocalPosition(m_position);
		m_vwp_enemyObject.lock()->transform->SetLocalRotationY_Degrees(m_enemyDirection);
		m_vwp_enemyObject.lock()->GetGameComponent<Enemy>()->SetAppearanceEffect(gameObject);
		auto objectTag = GameObjectTag(m_enemyTag);
		m_vwp_enemyObject.lock()->SetGameObjectTag(objectTag);
		//m_vlp_diamond.lock()->SetIsRemove(true);
		m_vlp_brightOffTimer->Start();//
	}
	else if(m_vlp_spawnAnimationTimer->IsOn())
	{
		
//		m_effectProgress +=  GameDevice::WorldSpeed* sec2Div;
		//m_vlp_diamond.lock()->transform->SetLocalScale(Vector3(m_effectProgress*m_diamondTargetScale));
		//m_vlp_diamond.lock()->transform->RollLocalRotationY_Degrees(10.0f * GameDevice::WorldSpeed);


		for (auto& drawInfo : m_list_appearanceDrawInformation) {
			drawInfo.lock()->Get().ExInfo.x =Easing::EaseOut( m_vlp_spawnAnimationTimer->GetPercent());
		}
	}
}

void ButiEngine::EnemySpawnPointComponent::OnSet()
{
	m_vlp_spawnAnimationTimer = ObjectFactory::Create<RelativeTimer>(120);
	m_vlp_brightOffTimer = ObjectFactory::Create<RelativeTimer>(30);
	m_position = Vector3(0, 0, 0);
	m_spawnType = 0;
	m_enemyTag = "Fly_0";
}

void ButiEngine::EnemySpawnPointComponent::Start()
{
	m_vlp_spawnAnimationTimer->Start();
	//m_vlp_diamond = GetManager().lock()->AddObjectFromCereal("EnemySpawnDiamond");
	//m_vlp_diamond.lock()->transform->SetLocalPosition(m_position);

	auto objectTag = GameObjectTag("");

	m_enemyDirection = static_cast<float>(ButiRandom::GetInt(-180, 180));
	switch (m_spawnType)
	{
	case 0: //ハエ
		m_diamondTargetScale = 0.8f;
		m_effectTargetScale = 1.5f;
		objectTag = GameObjectTag(m_enemyTag);
		m_vwp_appearnceObject= GetManager().lock()->AddObjectFromCereal("AppearnceEffect_Fly");


		break;
	case 1: //ストーカー
		m_diamondTargetScale = 0.8f;
		m_effectTargetScale = 1.5f;
		objectTag = GameObjectTag(m_enemyTag);
		m_vwp_appearnceObject = GetManager().lock()->AddObjectFromCereal("AppearnceEffect_Stalker");

		break;
	case 2: //キバ
		m_diamondTargetScale = 3.2f;
		m_effectTargetScale = 5.0f;
		objectTag = GameObjectTag(m_enemyTag);
		m_vwp_appearnceObject = GetManager().lock()->AddObjectFromCereal("AppearnceEffect_Kiba");
		break;
	case 3: //カザン
		m_diamondTargetScale = 4.0f;
		m_effectTargetScale = 7.0f;
		objectTag = GameObjectTag(m_enemyTag);
		m_vwp_appearnceObject = GetManager().lock()->AddObjectFromCereal("AppearnceEffect_Volcano");
		break;
	}

	auto drawcomponents = m_vwp_appearnceObject.lock()->GetGameComponents<MeshDrawComponent>();
	for (auto& drawComponent : drawcomponents) {
		m_list_appearanceDrawInformation.Add(drawComponent->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation"));
	}
	m_vwp_appearnceObject.lock()->transform->SetLocalPosition(m_position);
	m_vwp_appearnceObject.lock()->transform->SetLocalRotationY_Degrees(m_enemyDirection+180);
	m_vwp_appearnceObject.lock()->transform->SetLocalScale(Vector3(m_effectTargetScale));
	m_vwp_appearnceObject.lock()->transform->SetBaseTransform(gameObject.lock()->transform,true);
	m_vwp_appearnceObject.lock()->SetGameObjectTag(objectTag);
	//m_vlp_diamond.lock()->SetGameObjectTag(objectTag);
}

void ButiEngine::EnemySpawnPointComponent::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::EnemySpawnPointComponent::Clone()
{
	return ObjectFactory::Create<EnemySpawnPointComponent>();
}

void ButiEngine::EnemySpawnPointComponent::SetEnemyObject(Value_weak_ptr<GameObject> arg_vwp_enemy)
{
	m_vwp_enemyObject = arg_vwp_enemy;
}
