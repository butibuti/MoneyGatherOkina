#include "stdafx_u.h"
#include "Player.h"
#include "InputManager.h"
#include "ParticleGenerater.h"
#include "WaveManager.h"
#include "SphereExclusion.h"
#include "SeparateDrawObject.h"
#include "Enemy_Stalker.h"

void ButiEngine::Player::OnUpdate()
{
	TrajectoryParticleWaitCount();
	MoveKnockBack();
	Move();
	if (m_isInvincible)
	{
		OnInvincible();
	}
}

void ButiEngine::Player::OnSet()
{
	auto collisionLambda = std::function<void(Value_weak_ptr<GameObject>&)>([this](Value_weak_ptr<GameObject>& arg_vwp_other)->void
		{
			if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("DamageArea")))
			{
				OnCollisionDamageArea(arg_vwp_other);
			}
			else if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("Stalker")))
			{
				OnCollisionStalker(arg_vwp_other);
			}
		});

	gameObject.lock()->AddCollisionEnterReaction(collisionLambda);
}

void ButiEngine::Player::OnShowUI()
{
	GUI::Text("Level:%d", m_level);
	GUI::Text("Exp:%d", m_exp);
	GUI::Text("MaxWorker:%d", m_maxWorkerCount);
	GUI::Text("Life:%d", m_life);
	if (GUI::Button("LevelUp"))
	{
		LevelUp();
	}

	GUI::BulletText("Speed");
	GUI::DragFloat("##speed", &m_maxMoveSpeed, 0.01f, 0.0f, 1.0f);
	GUI::BulletText("VibrationForce");
	GUI::DragFloat("##vForce", &m_vibrationForce, 1.0f, 0.0f, 100.0f);
}

void ButiEngine::Player::Start()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->CreateDrawObject("Player");
	gameObject.lock()->GetGameComponent<SphereExclusion>()->SetMass(1.0f);

	m_vwp_waveManager = GetManager().lock()->GetGameObject("WaveManager").lock()->GetGameComponent<WaveManager>();

	m_life = 3;

	m_level = 1;
	m_maxLevel = 10;
	m_maxWorkerCount = 10;
	m_exp = 0;

	m_knockBackVelocity = Vector3(0, 0, 0);
	m_knockBackFrame = 0;
	m_maxKnockBackFrame = 15;
	m_isKnockBack = false;

	m_vlp_invincibleTimer = ObjectFactory::Create<RelativeTimer>(60);
	m_isInvincible = false;

	m_isDead = false;

	m_vlp_lookAt = gameObject.lock()->GetGameComponent<LookAtComponent>();
	m_vlp_lookAt->m_speed = 0.1f;
	m_vlp_camera = GetManager().lock()->GetScene().lock()->GetCamera("main");
	m_velocity = Vector3Const::Zero;
	m_maxMoveSpeed = 0.15f;
	m_acceleration = 0.01f;
	m_deceleration = 0.01f;

	m_vibrationForce = 3.0f;

	m_vwp_particleGenerater = GetManager().lock()->GetGameObject("ParticleController").lock()->GetGameComponent<ParticleGenerater>();
	m_addTrajectoryParticleCounter = 0;
	m_addTrajectoryParticleWait = 4;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Player::Clone()
{
	return ObjectFactory::Create<Player>();
}

void ButiEngine::Player::AddExp()
{
	if (m_level == m_maxLevel) { return; }

	m_exp++;
	std::uint16_t requestExp = CalculateRequestExp();
	if (m_exp == requestExp)
	{
		LevelUp();
	}
}

void ButiEngine::Player::KnockBack(const Vector3& arg_velocity)
{
	if (m_isKnockBack) return;
	if (m_vwp_waveManager.lock()->IsClearAnimationFlag()) return;

	//現在の移動量をゼロにする
	m_velocity = Vector3Const::Zero;
	//ノックバックの初期値セット
	m_isKnockBack = true;
	m_knockBackFrame = m_maxKnockBackFrame;

	m_knockBackVelocity = arg_velocity;
	m_knockBackVelocity.y = 0;
	//m_knockBackVelocity.x = ButiRandom::GetRandom(-10, 10, 100);
	//m_knockBackVelocity.z = ButiRandom::GetRandom(-10, 10, 100);
	m_knockBackVelocity.Normalize();
}

void ButiEngine::Player::Move()
{
	//ノックバック中は操作不能
	if (m_isKnockBack) return;
	//リザルト(クリア演出)中は操作不能
	if (m_vwp_waveManager.lock()->IsClearAnimationFlag()) return;

	//X、Z平面の移動方向を取得
	Vector2 leftStick = InputManager::GetLeftStick();
	
	auto cameraTransform = m_vlp_camera->vlp_transform;

	if (leftStick != 0)
	{
		Vector3 dir = leftStick.x * cameraTransform->GetRight() + leftStick.y * cameraTransform->GetFront();
		dir.y = 0.0f;

		/////////////////////////////////////////////
		auto lookTarget = gameObject.lock()->transform->Clone();
		lookTarget->Translate(dir);
		m_vlp_lookAt->m_vlp_lookTarget = lookTarget;
		/////////////////////////////////////////////

		m_velocity += dir.GetNormalize() * m_acceleration;
		if (m_velocity.GetLength() > m_maxMoveSpeed)
		{
			m_velocity = m_velocity.GetNormalize() * m_maxMoveSpeed;
		}
		//m_velocity.Normalize();

		if (m_addTrajectoryParticleCounter == 0)
		{
			auto position = gameObject.lock()->transform->GetWorldPosition();
			Vector3 pachiPachiPosition = position - m_velocity * 10.0f;
			m_vwp_particleGenerater.lock()->TrajectoryParticles(position);
			m_vwp_particleGenerater.lock()->PachiPachiParticles(pachiPachiPosition);
		}
	}
	else
	{
		if (m_velocity.GetLength() < m_deceleration)
		{
			m_velocity = Vector3Const::Zero;
		}
		m_velocity -= m_velocity.GetNormalize() * m_deceleration;
		m_velocity.y = 0;
	}

	gameObject.lock()->transform->Translate(m_velocity);
}

void ButiEngine::Player::LevelUp()
{
	m_level++;
	m_maxWorkerCount += 10;
}

void ButiEngine::Player::MoveKnockBack()
{
	if (!m_isKnockBack) return;

	//ノックバック中の処理
	gameObject.lock()->transform->Translate(m_knockBackVelocity);

	m_knockBackVelocity *= 0.7f;

	m_knockBackFrame--;

	if (m_knockBackFrame <= 0)
	{
		m_isKnockBack = false;
	}
}

void ButiEngine::Player::TrajectoryParticleWaitCount()
{
	if (m_addTrajectoryParticleCounter < m_addTrajectoryParticleWait)
	{
		m_addTrajectoryParticleCounter++;
	}
	else
	{
		m_addTrajectoryParticleCounter = 0;
	}
}

void ButiEngine::Player::Damage()
{
	m_life--;

	if (m_life == 0)
	{
		m_isDead = true;
		return;
	}

	m_isInvincible = true;
	m_vlp_invincibleTimer->Start();
}

void ButiEngine::Player::OnInvincible()
{
	if (m_vlp_invincibleTimer->Update())
	{
		m_isInvincible = false;
		m_vlp_invincibleTimer->Stop();
	}
}

void ButiEngine::Player::OnCollisionDamageArea(Value_weak_ptr<GameObject> arg_vwp_other)
{
	if (m_isInvincible) { return; }
	if (m_isDead) { return; }

	Vector3 velocity = gameObject.lock()->transform->GetLocalPosition() - arg_vwp_other.lock()->transform->GetWorldPosition();
	KnockBack(velocity);
	Damage();
}

void ButiEngine::Player::OnCollisionStalker(Value_weak_ptr<GameObject> arg_vwp_other)
{
	if (m_isInvincible) { return; }
	if (m_isDead) { return; }

	auto stalker = arg_vwp_other.lock()->GetGameComponent<Enemy_Stalker>();
	if (!stalker) { return; }

	bool isPrey = stalker->IsPrey();
	if (isPrey) { return; }

	Vector3 velocity = gameObject.lock()->transform->GetLocalPosition() - arg_vwp_other.lock()->transform->GetWorldPosition();
	KnockBack(velocity);
	Damage();
}

std::uint16_t ButiEngine::Player::CalculateRequestExp()
{
	return m_level * 10;
}
