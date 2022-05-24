#include"stdafx_u.h"
#include"FieldParticleEmitter.h"

void ButiEngine::FieldParticleEmitter::OnUpdate()
{
}

void ButiEngine::FieldParticleEmitter::OnSet()
{
}

void ButiEngine::FieldParticleEmitter::Start()
{
	if (m_vwp_particleController.lock()) {
		return;
	}
	m_vwp_particleController = GetManager().lock()->GetGameObject("PolygonParticleController").lock()->GetGameComponent<ImmediateParticleController>();
	
	Particle3D p;
	p.color = ButiColor::Amber();
	p.size = 5;
	float divXDensity = 1.0 / m_density.x, divYDensity = 1.0 / m_density.y, divZDensity = 1.0 / m_density.z;
	for (float x=-m_range.x; x < m_range.x; x+=divXDensity) {
		for (float y = 0; y < m_range.y*2; y += divYDensity) {
			for (float z = -m_range.z; z < m_range.z; z += divZDensity) {

				p.position =Vector3(x + ButiRandom::GetRandom(-m_fluctuation.x, m_fluctuation.x, 10), y + ButiRandom::GetRandom(-m_fluctuation.y, m_fluctuation.y, 10), z + ButiRandom::GetRandom(-m_fluctuation.z, m_fluctuation.z, 10));
				if (!ButiRandom::GetInt(0, 10)) {
					p.force = Vector3(ButiRandom::GetRandom(-1.0f, 1.0f, 10), ButiRandom::GetRandom(-1.0f, 1.0f, 10), ButiRandom::GetRandom(-1.0f, 1.0f, 10)).Normalize() * 0.01;
					p.accelation = 0.1f;
				}
				p.life = 100000;
				p.axis = Vector3(ButiRandom::GetRandom(-1.0f, 1.0f, 10), ButiRandom::GetRandom(-1.0f, 1.0f, 10), ButiRandom::GetRandom(-1.0f, 1.0f, 10)).Normalize();
				p.angle = ButiRandom::GetRandom(-1.0f, 1.0f, 10);
				m_vwp_particleController.lock()->AddParticle(p);
			}
		}


	}
}

void ButiEngine::FieldParticleEmitter::OnShowUI()
{
	GUI::DragFloat3("m_range", m_range, 0.1f, 0.1, 100);
	GUI::DragFloat3("m_density", m_density, 0.1f, 0, 100);
	GUI::DragFloat3("m_fluctuation", m_fluctuation, 0.1f, 0, 100);
	GUI::DragFloat("m_generatePase", m_generatePase,0.1, 0, 100);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FieldParticleEmitter::Clone()
{
	auto output = ObjectFactory::Create<FieldParticleEmitter>();
	output->m_density = m_density;
	output->m_generatePase = m_generatePase;
	output->m_range = m_range;
	return output;
}
