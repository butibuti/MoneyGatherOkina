#include "stdafx_u.h"
#include "SpriteParticleGenerator.h"

void ButiEngine::SpriteParticleGenerator::OnUpdate()
{
}

void ButiEngine::SpriteParticleGenerator::OnSet()
{
}

void ButiEngine::SpriteParticleGenerator::OnShowUI()
{
}

void ButiEngine::SpriteParticleGenerator::Start()
{
	m_vwp_spriteParticleController = gameObject.lock()->GetGameComponent<SpriteParticleController>();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::SpriteParticleGenerator::Clone()
{
	return ObjectFactory::Create<SpriteParticleGenerator>();
}

void ButiEngine::SpriteParticleGenerator::AttackFlashParticles(const Vector3& arg_position, const float arg_spawnRadius, const float arg_size, const Vector4& arg_color)
{
	Particle2D particle;

	Vector3 dir;
	dir.x = ButiRandom::GetInt(-100, 100);
	dir.y = ButiRandom::GetInt(-100, 100);
	dir.z = ButiRandom::GetInt(-100, 100);
	dir.Normalize();

	float spawnRadius = arg_spawnRadius * ButiRandom::GetRandom(0.3f, 1.0f);

	particle.position = arg_position + dir * spawnRadius;
	particle.angle = ButiRandom::GetRandom(0.0f, 1.0f, 100);
	particle.size = arg_size * ButiRandom::GetRandom(0.3f, 1.0f);
	particle.color = arg_color;
	particle.life = 12;

	m_vwp_spriteParticleController.lock()->AddParticle(particle);
}

void ButiEngine::SpriteParticleGenerator::ChargeParticles(const Vector3& arg_position, const float arg_radius, const Vector4& arg_color)
{
	Particle2D particle;

	std::int8_t maxParticleCount = 1;
	for (std::int8_t i = 0; i < maxParticleCount; i++)
	{
		Vector3 dir;
		dir.x = ButiRandom::GetInt(-100, 100);
		dir.y = ButiRandom::GetInt(-100, 100);
		dir.z = ButiRandom::GetInt(-100, 100);
		dir.Normalize();

		float spawnRadius = arg_radius * ButiRandom::GetRandom(0.3f, 1.0f);

		particle.position = arg_position + dir * spawnRadius;
		particle.velocity = -dir * 0.1f;
		particle.force = dir * 0.02f;
		particle.size = 10.0f;
		particle.sizePase = -0.1f;
		particle.life = 30;
		particle.color = arg_color;

		m_vwp_spriteParticleController.lock()->AddParticle(particle);
	}
}
