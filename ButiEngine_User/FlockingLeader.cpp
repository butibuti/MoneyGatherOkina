#include "stdafx_u.h"
#include "FlockingLeader.h"
#include "Flocking.h"
#include "GameSettings.h"
#include "SpriteParticleGenerator.h"
#include "InputManager.h"
#include "WaveManager.h"
#include "PauseManagerComponent.h"

void ButiEngine::FlockingLeader::OnUpdate()
{
	if (!m_vwp_waveManager.lock()->IsGameStart())
	{
		return;
	}
	if (!m_isDraw)
	{
		gameObject.lock()->GetGameComponent<MeshDrawComponent>()->ReRegist();
		m_isDraw = true;
	}
	m_prevPos = m_pos;
	m_pos = gameObject.lock()->transform->GetWorldPosition();

	if (IsStop())
	{
		return;
	}

	if (InputManager::IsTriggerGatherKey())
	{
		//GatherStart();
		CreateStarFlash();
	}
	else if (InputManager::IsReleaseGatherKey())
	{
		//DiffusionStart();
		CreateCircleFlash();
	}

	m_isGather = InputManager::IsPushGatherKey();
	if (m_isGather)
	{
		m_vwp_spriteParticleGenerator.lock()->GatherParticles(gameObject.lock()->transform, 5.0f, GameSettings::WORKER_COLOR);
	}
}

void ButiEngine::FlockingLeader::OnSet()
{
	auto tag = GameObjectTag("Leader");
	gameObject.lock()->SetGameObjectTag(tag);

	Flocking::ResetWorkers();
}

void ButiEngine::FlockingLeader::OnRemove()
{
	gameObject.lock()->RemoveGameObjectTag(GameObjectTag("Leader"));
}

void ButiEngine::FlockingLeader::OnShowUI()
{
}

void ButiEngine::FlockingLeader::Start()
{
	m_vwp_waveManager = GetManager().lock()->GetGameObject("WaveManager").lock()->GetGameComponent<WaveManager>();
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManagerComponent>();
	m_vwp_spriteParticleGenerator = GetManager().lock()->GetGameObject("SphereParticleController").lock()->GetGameComponent<SpriteParticleGenerator>();
	gameObject.lock()->GetGameComponent<MeshDrawComponent>()->UnRegist();
	m_pos = gameObject.lock()->transform->GetWorldPosition();
	m_prevPos = m_pos;

	m_isDraw = false;
	m_isGather = true;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FlockingLeader::Clone()
{
	return ObjectFactory::Create<FlockingLeader>();
}

void ButiEngine::FlockingLeader::GatherStart()
{
	auto vec = Flocking::GetWorkers();
	auto end = vec.end();
	for (auto itr = vec.begin(); itr != end; ++itr)
	{
		(*itr)->GetGameComponent<Flocking>()->GatherStart();
	}
}

void ButiEngine::FlockingLeader::DiffusionStart()
{
	auto vec = Flocking::GetWorkers();
	auto end = vec.end();
	for (auto itr = vec.begin(); itr != end; ++itr)
	{
		(*itr)->GetGameComponent<Flocking>()->DiffusionStart();
	}
}

void ButiEngine::FlockingLeader::CreateCircleFlash()
{
	auto circleFlash = GetManager().lock()->AddObjectFromCereal("CircleFlash");
	circleFlash.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetWorldPosition());
	circleFlash.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
}

void ButiEngine::FlockingLeader::CreateStarFlash()
{
	auto circleFlash = GetManager().lock()->AddObjectFromCereal("StarFlash");
	circleFlash.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetWorldPosition());
	circleFlash.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
}

bool ButiEngine::FlockingLeader::IsStop()
{
	if (m_vwp_waveManager.lock()->IsEvent()) { return true; }
	if (m_vwp_pauseManager.lock()->IsPause()) { return true; }
	
	return false;
}
