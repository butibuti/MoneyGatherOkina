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
	m_maxLifeOffset = m_maxLife - m_minLife;
	m_sizePaseDiv = 1.0f / m_sizePase;
	m_vwp_particleController.lock()->AddParticleControlFunction("FieldParticle", std::function<void(Particle3D&)>([this](Particle3D& arg_particle)->void {

		arg_particle.position += arg_particle.velocity * GameDevice::WorldSpeed;
		arg_particle.angle += arg_particle.anglePase * GameDevice::WorldSpeed;
		arg_particle.life -= 1.0f * GameDevice::WorldSpeed;
		
		if (arg_particle.life <=m_size* m_sizePaseDiv) {
			arg_particle.size = arg_particle.life * m_sizePase;
		}
		else if (arg_particle.size < m_size) {
			arg_particle.size += m_sizePase;
		}
		if (arg_particle.life <= 0) {
			this->CreateParticle(arg_particle.force);
		}

	}));

	controlIndex = m_vwp_particleController.lock()->GetControlFunctionIndex("FieldParticle");
	Particle3D p;
	p.color = m_color;
	p.size = m_size;
	p.controlIndex = controlIndex;
	float divXDensity = 1.0 / m_density.x, divYDensity = 1.0 / m_density.y, divZDensity = 1.0 / m_density.z;
	for (float x=-m_range.x; x < m_range.x; x+=divXDensity) {
		for (float y = 0; y < m_range.y*2; y += divYDensity) {
			for (float z = -m_range.z; z < m_range.z; z += divZDensity) {

				p.position =Vector3(x + ButiRandom::GetRandom(-m_fluctuation.x, m_fluctuation.x, 10), y + ButiRandom::GetRandom(-m_fluctuation.y, m_fluctuation.y, 10), z + ButiRandom::GetRandom(-m_fluctuation.z, m_fluctuation.z, 10));
				p.force = p.position;
				p.velocity = Vector3(ButiRandom::GetRandom(-1.0f, 1.0f, 10), ButiRandom::GetRandom(-1.0f, 1.0f, 10), ButiRandom::GetRandom(-1.0f, 1.0f, 10)).Normalize() * 0.01;
				p.life = m_minLife*2+ ButiRandom::GetRandom(0.0f, m_maxLifeOffset);
				p.axis = Vector3(ButiRandom::GetRandom(-1.0f, 1.0f, 10), ButiRandom::GetRandom(-1.0f, 1.0f, 10), ButiRandom::GetRandom(-1.0f, 1.0f, 10)).Normalize();
				p.angle = ButiRandom::GetRandom(-1.0f, 1.0f, 10);
				p.anglePase =ButiRandom::GetRandom(0,1)? 0.005:-0.005*ButiRandom::GetRandom(m_minAngleSpeed,m_maxAngleSpeed, 10);
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
	GUI::ColorEdit4("m_color", m_color);
	GUI::DragFloat("m_maxAngleSpeed", m_maxAngleSpeed, 0.1f, 0, 10);
	GUI::DragFloat("m_minAngleSpeed", m_minAngleSpeed, 0.1f, 0, 10);
	bool isLifeEdit = 	GUI::DragFloat("m_maxLife", m_maxLife, 0.1f, 0, 10);
	isLifeEdit|= GUI::DragFloat("m_minLife", m_minLife, 0.1f, 0, 10);
	if (isLifeEdit) {
		m_maxLifeOffset = m_maxLife - m_minLife;
	}
	if (GUI::DragFloat("m_sizePase", m_sizePase, 0.1f, 0, 10)) {
		m_sizePaseDiv = 1.0f / m_sizePase;
	}
	GUI::DragFloat("m_size", m_size, 0.1f, 0, 10);

}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FieldParticleEmitter::Clone()
{
	auto output = ObjectFactory::Create<FieldParticleEmitter>();
	output->m_density = m_density;
	output->m_range = m_range;
	output->m_color = m_color;
	output->m_fluctuation = m_fluctuation; 
	output->m_maxAngleSpeed = m_maxAngleSpeed;
	output->m_maxLife = m_maxLife;
	output->m_minAngleSpeed = m_minAngleSpeed;
	output->m_minLife = m_minLife;
	output->m_sizePase = m_sizePase;
	output->m_size = m_size;
	
	return output;
}

void ButiEngine::FieldParticleEmitter::CreateParticle(const Vector3& arg_position)
{
	Particle3D p;
	p.size = 0;
	p.color = m_color;
	p.position = arg_position;
	p.force = arg_position;
	p.velocity = Vector3(ButiRandom::GetRandom(-1.0f, 1.0f, 10), ButiRandom::GetRandom(-1.0f, 1.0f, 10), ButiRandom::GetRandom(-1.0f, 1.0f, 10)).Normalize() * 0.01;
	p.life = m_minLife + ButiRandom::GetRandom(0.0f, m_maxLifeOffset);
	p.axis = Vector3(ButiRandom::GetRandom(-1.0f, 1.0f, 10), ButiRandom::GetRandom(-1.0f, 1.0f, 10), ButiRandom::GetRandom(-1.0f, 1.0f, 10)).Normalize();
	p.angle = ButiRandom::GetRandom(-1.0f, 1.0f, 10);
	p.anglePase = ButiRandom::GetRandom(0, 1) ? 0.005 : -0.005 * ButiRandom::GetRandom(m_minAngleSpeed, m_maxAngleSpeed, 10);
	p.controlIndex = controlIndex;
	m_vwp_particleController.lock()->AddParticle(p);
}
