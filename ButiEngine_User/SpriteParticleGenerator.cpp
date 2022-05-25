#include "stdafx_u.h"
#include "SpriteParticleGenerator.h"
#include "GameSettings.h"

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

	m_vwp_spriteParticleController.lock()->AddParticleControlFunction("GatherControl", std::function<void(Particle2D&)>([](Particle2D& arg_particle) {
		float progress = arg_particle.frame / arg_particle.angle;
		Vector3 p1 = MathHelper::LerpPosition(arg_particle.force, arg_particle.velocity, progress);
		Vector3 p2 = MathHelper::LerpPosition(arg_particle.velocity, Vector3Const::Zero, progress);
		arg_particle.position = MathHelper::LerpPosition(p1, p2, progress);
		//arg_particle.position = MathHelper::LerpPosition(arg_particle.position, Vector3Const::Zero, arg_particle.velocity.x);
		arg_particle.size = MathHelper::Lerp(arg_particle.anglePase, arg_particle.sizePase, progress);
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

	std::int8_t maxParticleCount = 8;
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

		//生成地点
		particle.position = dir * spawnRadius;
		particle.force = particle.position;
		Vector3 verticalVec = Vector3(-dir.z, dir.y, dir.x);
		//ベジェ曲線制御点
		particle.velocity = particle.position * 0.5f + verticalVec * 5.0f;
		//親
		particle.m_parentTransform = arg_vwp_transform.lock();
		//生成サイズ
		particle.anglePase = 0.3f;
		//最終サイズ
		particle.sizePase = 5.0f;
		//ライフ
		particle.life = 35;
		//補完フレーム
		particle.angle = 30;
		//色
		particle.color = arg_color;

		std::int8_t rand = ButiRandom::GetInt(1, 20);
		if (rand <= 1)
		{
			//20分の1の確率で大きい白出す
			particle.anglePase *= 1.5f;
			particle.sizePase *= 1.5f;
			particle.color = ButiColor::White();
		}
		else if (rand <= 3)
		{
			//20分の2の確率で青出す
			particle.anglePase *= 1.5f;
			particle.sizePase *= 1.2f;
			particle.color = ButiColor::Blue();
		}

		particle.controlIndex = m_vwp_spriteParticleController.lock()->GetControlFunctionIndex("GatherControl");

		m_vwp_spriteParticleController.lock()->AddParticle(particle);
	}
}
