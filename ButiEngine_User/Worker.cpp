#include "stdafx_u.h"
#include "Worker.h"

void ButiEngine::Worker::OnUpdate()
{
}

void ButiEngine::Worker::OnSet()
{
}

void ButiEngine::Worker::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Worker::Clone()
{
	return ObjectFactory::Create<Worker>();
}
