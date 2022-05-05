#include "stdafx_u.h"
#include "ParticleGenerater.h"

#include "InputManager.h"

void ButiEngine::ParticleGenerater::OnUpdate()
{
	if (InputManager::IsTriggerRightKey())
	{
		Explosion(Vector3(0, 5, 0));
	}
	if (InputManager::IsPushAnyGameKey())
	{
		SparkParticle(Vector3(0, 10, 0));
	}

	Flickering();
}

void ButiEngine::ParticleGenerater::OnSet()
{
}

void ButiEngine::ParticleGenerater::OnShowUI()
{
}

void ButiEngine::ParticleGenerater::Start()
{
	if (m_vwp_immediateParticleController.lock() == nullptr)
	{
		m_vwp_immediateParticleController = gameObject.lock()->GetGameComponent<ImmediateParticleController>();
	}
	m_flickeringFrame = 0;
	m_isChangeColor = false;
}

void ButiEngine::ParticleGenerater::Explosion(const Vector3& arg_position)
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

	//オレンジ
	particle.color = ButiColor::Orange(ButiColor::ShadeIndex::Shade_6);

	Vector3 velocity;
	velocity.x = (float)ButiRandom::GetRandom(-50, 50, 100);
	velocity.y = (float)ButiRandom::GetRandom( 20, 50, 100);
	velocity.z = (float)ButiRandom::GetRandom(-50, 50, 100);
	particle.velocity = velocity.Normalize() * speed;

	particle.force = velocity * 0.1f + Vector3(0, 0.6f, 0);
	particle.accelation = 0.6f;

	m_vwp_immediateParticleController.lock()->AddParticle(particle);

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
