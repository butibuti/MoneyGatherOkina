#include "stdafx_u.h"
#include "SpriteParticleGenerator.h"
#include "GameSettings.h"
#include "WaveManager.h"

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
	m_vwp_waveManager = GetManager().lock()->GetGameObject("WaveManager").lock()->GetGameComponent<WaveManager>();
	m_vwp_spriteParticleController = gameObject.lock()->GetGameComponent<SpriteParticleController>();

	m_vwp_spriteParticleController.lock()->AddParticleControlFunction("GatherControl", std::function<void(Particle2D&)>([](Particle2D& arg_particle) {
		float progress = arg_particle.frame / arg_particle.angle;
		Vector3 p1 = MathHelper::LerpPosition(arg_particle.force, arg_particle.velocity, progress);
		Vector3 p2 = MathHelper::LerpPosition(arg_particle.velocity, Vector3Const::Zero, progress);
		arg_particle.position = MathHelper::LerpPosition(p1, p2, progress);
		//arg_particle.position = MathHelper::LerpPosition(arg_particle.position, Vector3Const::Zero, arg_particle.velocity.x);
		arg_particle.size = MathHelper::Lerp(arg_particle.anglePase, arg_particle.sizePase, progress);
		//arg_particle.color = MathHelper::LerpPosition(arg_particle.color, arg_particle.colorPase, 0.03f);
		arg_particle.life -= 1.0f * GameDevice::WorldSpeed;
		arg_particle.frame += 1.0f * GameDevice::WorldSpeed;
		if ((arg_particle.position).GetLength() < 0.5f) {
			//arg_particle.life = 0;
		}
		}));
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::SpriteParticleGenerator::Clone()
{
	return ObjectFactory::Create<SpriteParticleGenerator>();
}

void ButiEngine::SpriteParticleGenerator::AttackFlashParticles(const Vector3& arg_position, const float arg_spawnRadius, const float arg_size, const Vector4& arg_color)
{
	if (m_vwp_waveManager.lock()->IsClearAnimation() || m_vwp_waveManager.lock()->IsGameOver())
	{
		return;
	}

	Particle2D particle;

	Vector3 dir;
	dir.x = ButiRandom::GetInt(-100, 100);
	dir.y = ButiRandom::GetInt(-100, 100);
	dir.z = ButiRandom::GetInt(-100, 100);
	if (dir == Vector3Const::Zero)
	{
		dir.x = 1;
	}
	dir.Normalize();

	float spawnRadius = arg_spawnRadius * ButiRandom::GetRandom(0.3f, 1.0f, 10);

	particle.position = arg_position + dir * spawnRadius;
	particle.angle = ButiRandom::GetRandom(0.0f, 1.0f, 100);
	particle.size = arg_size * ButiRandom::GetRandom(0.7f, 1.0f, 10);
	particle.color = arg_color;
	particle.life = 12;

	m_vwp_spriteParticleController.lock()->AddParticle(particle);
}

void ButiEngine::SpriteParticleGenerator::GatherParticles(Value_weak_ptr<Transform> arg_vwp_transform, const float arg_radius, const Vector4& arg_color)
{
	Particle2D particle;

	std::int8_t maxParticleCount = 4;
	for (std::int8_t i = 0; i < maxParticleCount; i++)
	{
		Vector3 dir;
		dir.x = ButiRandom::GetInt(-100, 100);
		dir.y = 0.0f;
		dir.z = ButiRandom::GetInt(-100, 100);
		if (dir == Vector3Const::Zero)
		{
			dir.x = 1;
		}
		dir.Normalize();

		//Vector3 pos = arg_vwp_transform.lock()->GetWorldPosition();
		float spawnRadius = arg_radius * ButiRandom::GetRandom(0.5f, 1.5f, 10);

		//?????n?_
		particle.position = dir * spawnRadius;
		particle.force = particle.position;
		Vector3 verticalVec = Vector3(-dir.z, dir.y, dir.x);
		//?x?W?F?????????_
		particle.velocity = particle.position * 0.5f + verticalVec * 5.0f;
		//?e
		particle.m_parentTransform = arg_vwp_transform.lock();
		//?????T?C?Y
		particle.anglePase = 0.3f;
		//???I?T?C?Y
		particle.sizePase = 5.0f;
		//???C?t
		particle.life = 35;
		//?????t???[??
		particle.angle = 30;
		//?F
		particle.color = arg_color;
		particle.colorPase = GameSettings::PLAYER_COLOR;

		std::int8_t rand = ButiRandom::GetInt(1, 20);
		if (rand <= 1)
		{
			//20????1???m?????????????o??
			particle.anglePase *= 1.5f;
			particle.sizePase *= 1.5f;
			particle.color = ButiColor::White();
		}
		else if (rand <= 3)
		{
			//20????2???m???????o??
			particle.anglePase *= 1.5f;
			particle.sizePase *= 1.2f;
			particle.color = ButiColor::Blue();
		}

		particle.controlIndex = m_vwp_spriteParticleController.lock()->GetControlFunctionIndex("GatherControl");

		m_vwp_spriteParticleController.lock()->AddParticle(particle);
	}
}

void ButiEngine::SpriteParticleGenerator::SpawnParticles(const Vector3& arg_position, const Vector4& arg_color)
{
	for (std::int8_t i = 0; i < 10; i++)
	{
		Particle2D particle;

		particle.position = arg_position;

		particle.life = 30;
		particle.size = 5.0f;
		particle.sizePase = -particle.size / particle.life;
		particle.color = arg_color;

		Vector3 velocity;
		velocity.x = (float)ButiRandom::GetRandom(-50, 50, 100);
		velocity.y = (float)ButiRandom::GetRandom(50, 100, 100);
		velocity.z = (float)ButiRandom::GetRandom(-50, 50, 100);
		particle.velocity = velocity.Normalize() * ButiRandom::GetRandom(0.15f, 0.3f, 100);
		particle.force = Vector3(0.0f, -0.05f, 0.0f);

		m_vwp_spriteParticleController.lock()->AddParticle(particle);
	}

	std::int8_t maxParticleCount = 16;
	for (std::int8_t i = 0; i < maxParticleCount; i++)
	{
		Particle2D particle;
		particle.position = arg_position;
		particle.position.y += 0.25f;

		float speed = 0.0f;

		particle.life = 20;
		particle.size = 8.0f;
		particle.sizePase = -particle.size / particle.life;

		particle.color = arg_color;

		Vector2 result;
		float angle = 360.0f / (float)maxParticleCount;
		float radian = angle * i * (3.1415f / 180);
		result = Vector2(cos(radian), sin(radian));
		result.Normalize();

		Vector2 vel = result;
		particle.velocity = Vector3(vel.x, 0, vel.y) * 0.2f;

		m_vwp_spriteParticleController.lock()->AddParticle(particle);
	}
}

void ButiEngine::SpriteParticleGenerator::AfterImageParticles(const Vector3& arg_position)
{
	Particle2D particle;

	Vector3 dir;
	dir.x = ButiRandom::GetInt(-100, 100);
	dir.y = ButiRandom::GetInt(-100, 100);
	dir.z = ButiRandom::GetInt(-100, 100);
	if (dir == Vector3Const::Zero)
	{
		dir.x = 1;
	}
	dir.Normalize();

	float spawnRadius = ButiRandom::GetRandom(0.3f, 0.7f, 10);

	particle.position = arg_position + dir * spawnRadius;
	particle.angle = ButiRandom::GetRandom(0.0f, 1.0f, 100);
	particle.life = 30;
	particle.size = ButiRandom::GetRandom(8.0f, 12.0f, 10);
	particle.sizePase = -particle.size / particle.life;
	particle.color = GameSettings::WORKER_COLOR;
	//particle.colorPase.w = -particle.color.w / particle.life;

	m_vwp_spriteParticleController.lock()->AddParticle(particle);
}
