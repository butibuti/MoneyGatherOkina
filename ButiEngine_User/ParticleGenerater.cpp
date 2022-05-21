#include "stdafx_u.h"
#include "ParticleGenerater.h"

#include "InputManager.h"

void ButiEngine::ParticleGenerater::OnUpdate()
{
	//if (InputManager::IsTriggerRightKey())
	//{
	//	ExplosionParticles(Vector3(0, 5, 0));
	//}
	//if (InputManager::IsPushAnyGameKey())
	//{
	//	SparkParticle(Vector3(0, 10, 0));
	//}
	//if (InputManager::IsTriggerPauseKey())
	//{
	//	CatchParticles(Vector3(0, 0, 0));
	//}

	Flickering();
}

void ButiEngine::ParticleGenerater::OnSet()
{
}

void ButiEngine::ParticleGenerater::OnShowUI()
{
	GUI::DragFloat("##accel", m_accel, 0.01f, -5, 5);
}

void ButiEngine::ParticleGenerater::Start()
{
	if (!m_vwp_immediateParticleController.lock())
	{
		m_vwp_immediateParticleController = gameObject.lock()->GetGameComponent<ImmediateParticleController>();
	}
	m_flickeringFrame = 0;
	m_isChangeColor = false;
}

void ButiEngine::ParticleGenerater::ExplosionParticles(const Vector3& arg_position)
{
	Particle3D particle;

	for (std::int8_t i = 0; i < 25; i++)
	{
		particle.position = arg_position;
		
		float speed = 0.0f;

		particle.life = 20;
		particle.axis = Vector3(ButiRandom::GetInt(1, 10) * 0.1f, 
			ButiRandom::GetInt(1, 10) * 0.1f, ButiRandom::GetInt(1, 10) * 0.1f);
		particle.angle = ButiRandom::GetInt(0, 90);
		particle.anglePase = ButiRandom::GetInt(1, 5) * 0.01f;
		particle.sizePase = 0;
		
		if (i == 0)
		{
			//明滅するやつ
			particle.color = ButiColor::Yellow(ButiColor::ShadeIndex::Shade_9);
			particle.size = 20.0f;
			particle.sizePase = -1.0f;
		}
		else if (i % 2)
		{
			//オレンジ
			particle.color = ButiColor::Orange(ButiColor::ShadeIndex::Shade_8);
			particle.life = ButiRandom::GetInt(40, 50);
			speed = 0.05f;
			particle.size = 5.0f;
		}
		else
		{
			//黄色
			particle.color = ButiColor::Yellow(ButiColor::ShadeIndex::Shade_4);
			particle.life = ButiRandom::GetInt(20, 30);
			speed = 0.15f;
			particle.size = 3.0f;
		}

		Vector3 velocity;
		velocity.x = (float)ButiRandom::GetRandom(-50, 50, 100);
		velocity.y = (float)ButiRandom::GetRandom(-50, 50, 100);
		velocity.z = (float)ButiRandom::GetRandom(-50, 50, 100);
		particle.velocity = velocity.Normalize() * speed;

		m_vwp_immediateParticleController.lock()->AddParticle(particle);
	}

}

void ButiEngine::ParticleGenerater::SparkParticle(const Vector3& arg_position)
{
	Particle3D particle;

	particle.position = arg_position;

	float speed = ButiRandom::GetInt(1, 4) * 0.05f;

	particle.life = 15;
	particle.axis = Vector3(ButiRandom::GetInt(1, 10) * 0.1f,
		ButiRandom::GetInt(1, 10) * 0.1f, ButiRandom::GetInt(1, 10) * 0.1f);
	particle.angle = ButiRandom::GetInt(0, 90);
	particle.anglePase = ButiRandom::GetInt(1, 5) * 0.01f;
	particle.sizePase = -0.2f;
	particle.size = 3.0f;

	if (speed < 0.15f)
	{
		//オレンジ
		particle.color = ButiColor::Orange(ButiColor::ShadeIndex::Shade_9);
	}
	else
	{
		//ほぼ黄色
		particle.color = ButiColor::Orange(ButiColor::ShadeIndex::Shade_5);
	}

	Vector3 velocity;
	velocity.x = (float)ButiRandom::GetRandom(-50, 50, 100);
	velocity.y = (float)ButiRandom::GetRandom(-50, 50, 100);
	velocity.z = (float)ButiRandom::GetRandom(-50, 50, 100);
	particle.velocity = velocity.Normalize() * speed;

	particle.force = velocity * 0.1f;
	particle.accelation = 0.6f;

	m_vwp_immediateParticleController.lock()->AddParticle(particle);

}

void ButiEngine::ParticleGenerater::TrajectoryParticles(const Vector3& arg_position)
{
	Particle3D particle;

	Vector3 randomPos = Vector3(ButiRandom::GetInt(-10, 10), ButiRandom::GetInt(-10, 10),
		ButiRandom::GetInt(-10, 10)) * 0.075f;
	particle.position = arg_position + randomPos;

	float speed = 0.025f;

	particle.life = 60;
	auto randomAxis = ButiRandom::GetInt(1, 10) * 0.1f;
	particle.axis = Vector3(randomAxis, randomAxis, randomAxis);
	particle.angle = ButiRandom::GetInt(0, 90);
	particle.anglePase = ButiRandom::GetInt(1, 5) * 0.02f;
	particle.size = 3.0f;
	particle.sizePase = -0.05f;

	//ほぼ黄色
	particle.color = ButiColor::Orange(ButiColor::ShadeIndex::Shade_5);
	

	Vector3 velocity;
	velocity.x = (float)ButiRandom::GetRandom(-50, 50, 100);
	velocity.y = (float)ButiRandom::GetRandom(-50, 50, 100);
	velocity.z = (float)ButiRandom::GetRandom(-50, 50, 100);
	particle.velocity = velocity.Normalize() * speed;

	m_vwp_immediateParticleController.lock()->AddParticle(particle);
}

void ButiEngine::ParticleGenerater::PachiPachiParticles(const Vector3& arg_position)
{


	for (std::int8_t i = 0; i < 5; i++)
	{
		Particle3D particle;
		Vector3 randomPos = Vector3(ButiRandom::GetInt(-10, 10), ButiRandom::GetInt(-10, 10),
			ButiRandom::GetInt(-10, 10)) * 0.1f;
		particle.position = arg_position + randomPos;

		float speed = 0.0f;

		particle.life = 20;
		//particle.axis = Vector3(ButiRandom::GetInt(1, 10) * 0.1f,
		//	ButiRandom::GetInt(1, 10) * 0.1f, ButiRandom::GetInt(1, 10) * 0.1f);
		particle.angle = ButiRandom::GetInt(0, 90);
		particle.anglePase = ButiRandom::GetInt(1, 5) * 0.05f;
		particle.sizePase = 0;
		particle.color = Vector4(1, 1, 1, 1);

		if (i == 0)
		{
			//中央のやつ
			particle.size = 3.0f;
			particle.sizePase = -0.15f;
			speed = 0.001f;
		}
		else
		{
			//はじけるやつ			
			particle.life = 10;
			speed = 0.15f;
			particle.sizePase = -0.01f;
			particle.size = 1.0f;
		}

		Vector3 velocity;
		velocity.x = (float)ButiRandom::GetRandom(-50, 50, 100);
		velocity.y = (float)ButiRandom::GetRandom(-50, 50, 100);
		velocity.z = (float)ButiRandom::GetRandom(-50, 50, 100);
		particle.velocity = velocity.Normalize() * speed;

		m_vwp_immediateParticleController.lock()->AddParticle(particle);
	}
}

void ButiEngine::ParticleGenerater::CatchParticles(const Vector3& arg_position, Value_ptr<ButiEngine::Transform> arg_vlp_parentTransform)
{
	Particle3D particle;

	std::int8_t maxParticleCount = 16;
	for (std::int8_t i = 0; i < maxParticleCount; i++)
	{
		particle.position = arg_position;

		float speed = 0.0f;

		particle.life = 20;
		particle.size = 2.0f;
		//particle.sizePase = -particle.size / particle.life;

		particle.color = Vector4(1, 1, 1, 1);

		Vector2 result;
		float angle = 360.0f / (float)maxParticleCount;
		float radian = angle * i * (3.1415f / 180);
		result = Vector2(cos(radian), sin(radian));
		result.Normalize();

		Vector2 vel = result;
		particle.velocity = Vector3(vel.x, 0, vel.y) * 0.2f;

		particle.force = -particle.velocity * 0.2f;
		particle.accelation = 1.10f;

		m_vwp_immediateParticleController.lock()->AddParticle(particle, arg_vlp_parentTransform);
	}

}

void ButiEngine::ParticleGenerater::ExplosionPolygonParticles(const Vector3& arg_position, const bool arg_isBig)
{
	Particle3D particle;

	float explosionScale = 1.0f;
	if (arg_isBig)
	{
		explosionScale = 1.0f;
	}
	else
	{
		explosionScale = 0.75f;
	}

	for (std::int8_t i = 0; i < 60; i++)
	{
		Vector3 randomPos = Vector3(ButiRandom::GetInt(-10, 10), ButiRandom::GetInt(-10, 10),
			ButiRandom::GetInt(-10, 10)) * 0.1f;
		particle.position = arg_position + randomPos;

		float speed = 0.0f;
		float accel = 0;

		particle.axis = Vector3(ButiRandom::GetInt(1, 10) * 0.1f,
			ButiRandom::GetInt(1, 10) * 0.1f, ButiRandom::GetInt(1, 10) * 0.1f);
		particle.angle = ButiRandom::GetInt(0, 90);
		particle.anglePase = ButiRandom::GetInt(1, 5) * 0.01f;
		particle.sizePase = 0;

		if (i % 3 == 2)
		{
			//オレンジ
			particle.color = ButiColor::Orange(ButiColor::ShadeIndex::Shade_8);
			speed = 0.3f;
			particle.size = 7.0f;
			accel = 1.15f;
			particle.life = 60;
		}
		else if (i % 3 == 1)
		{
			//黄色
			particle.color = ButiColor::Yellow(ButiColor::ShadeIndex::Shade_4);
			speed = 0.45f;
			particle.size = 5.0f;
			accel = 1.0925f;
			particle.life = 120;
		}
		else
		{
			//白色
			particle.color = Vector4(1, 1, 1, 1);
			speed = 0.9f;
			particle.size = 2.5f;
			accel = 1.0925f;
			particle.life = 180;
		}
		//particle.sizePase = -(particle.size / particle.life);

		particle.size *= explosionScale;

		Vector3 velocity;
		velocity.x = (float)ButiRandom::GetRandom(-50, 50, 100);
		velocity.y = (float)ButiRandom::GetRandom(-50, 50, 100);
		velocity.z = (float)ButiRandom::GetRandom(-50, 50, 100);
		particle.velocity = velocity.Normalize() * speed * explosionScale;

		particle.force = Vector3(0, -0.05, 0);
		particle.accelation = accel;

		m_vwp_immediateParticleController.lock()->AddParticle(particle);
	}
}

void ButiEngine::ParticleGenerater::Flickering()
{
	if (m_flickeringFrame < 2)
	{
		m_flickeringFrame++;
	}
	else
	{
		m_flickeringFrame = 0;
		m_isChangeColor = !m_isChangeColor;
	}

	//ここで明滅したいパーティクルの参照を取り、明滅するように更新をかける(？)
	auto& particleList = m_vwp_immediateParticleController.lock()->GetParticles();

	for (auto itr = particleList.begin(); itr != particleList.end(); ++itr)
	{
		if ((*itr).velocity != Vector3(0, 0, 0))
		{
			//明滅させたくないパーティクルは無視
			continue;
		}

		if (!m_isChangeColor)
		{
			(*itr).color = Vector4(1, 1, 1, 1);
		}
		else
		{
			(*itr).color = ButiColor::Yellow(ButiColor::ShadeIndex::Shade_9);
		}	
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::ParticleGenerater::Clone()
{
	return ObjectFactory::Create<ParticleGenerater>();
}
