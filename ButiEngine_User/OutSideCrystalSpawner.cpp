#include "stdafx_u.h"
#include "OutSideCrystalSpawner.h"
#include "OutsideCrystal.h"

void ButiEngine::OutsideCrystalSpawner::OnUpdate()
{
}

void ButiEngine::OutsideCrystalSpawner::OnSet()
{
}

void ButiEngine::OutsideCrystalSpawner::OnRemove()
{
}

void ButiEngine::OutsideCrystalSpawner::OnShowUI()
{
}

void ButiEngine::OutsideCrystalSpawner::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::OutsideCrystalSpawner::Clone()
{
	return ObjectFactory::Create<OutsideCrystalSpawner>();
}

void ButiEngine::OutsideCrystalSpawner::ActivePhase(const std::int32_t arg_index)
{
	if (arg_index >= m_vec_crystals.size()) { return; }
	auto vec = m_vec_crystals[arg_index];
}

void ButiEngine::OutsideCrystalSpawner::CreateCrystals()
{
}
