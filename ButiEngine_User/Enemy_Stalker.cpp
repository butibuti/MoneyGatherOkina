#include "stdafx_u.h"
#include "Enemy_Stalker.h"
#include "Enemy.h"

void ButiEngine::Enemy_Stalker::OnUpdate()
{
}

void ButiEngine::Enemy_Stalker::OnSet()
{
}

void ButiEngine::Enemy_Stalker::OnRemove()
{
}

void ButiEngine::Enemy_Stalker::OnShowUI()
{
}

void ButiEngine::Enemy_Stalker::Start()
{
	SetEnemyParameter();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Enemy_Stalker::Clone()
{
	return ObjectFactory::Create<Enemy_Stalker>();
}

void ButiEngine::Enemy_Stalker::OnCollisionPlayer()
{
}

void ButiEngine::Enemy_Stalker::OnCollisionWorker()
{
}

void ButiEngine::Enemy_Stalker::OnCollisionEnemy()
{
}

void ButiEngine::Enemy_Stalker::SetEnemyParameter()
{
	auto enemyComponent = gameObject.lock()->GetGameComponent<Enemy>();
	enemyComponent->CreatePocket(3);
	enemyComponent->SetNearBorder(gameObject.lock()->transform->GetLocalScale().x * 0.5f + 1.0f);
	enemyComponent->SetVibrationCapacity(100.0f);
	enemyComponent->SetVibrationResistance(3.0f);
}
