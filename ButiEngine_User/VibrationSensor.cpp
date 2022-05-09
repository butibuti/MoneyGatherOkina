#include "stdafx_u.h"
#include "VibrationSensor.h"

void ButiEngine::VibrationSensor::OnUpdate()
{
}

void ButiEngine::VibrationSensor::OnSet()
{
}

void ButiEngine::VibrationSensor::OnShowUI()
{
}

void ButiEngine::VibrationSensor::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::VibrationSensor::Clone()
{
	return ObjectFactory::Create<VibrationSensor>();
}
