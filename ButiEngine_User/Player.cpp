#include "stdafx_u.h"
#include "Player.h"
#include "InputManager.h"
#include "ParticleGenerater.h"
#include "WaveManager.h"
#include "SphereExclusion.h"
#include "SeparateDrawObject.h"
#include "Enemy_Stalker.h"
#include "ShockWave.h"
#include "VibrationEffectComponent.h"
#include "BeeSoulPodUIComponent.h"
#include "FloatMotionComponent.h"
#include "TiltMotion.h"
#include "ShakeComponent.h"
#include "CameraShakeComponent.h"
#include "NumberManagerComponent.h"

void ButiEngine::Player::OnUpdate()
{
	if (m_isIncrease)
	{
		IncreaseVibration();
	}
	else
	{
		DecreaseVibration();
	}
	VibrationEffect();

	m_nearEnemyCount = 0;

	MoveKnockBack();
	Move();

	if (m_isInvincible)
	{
		OnInvincible();
	}

	VibrationPowerDrawUpdate();
	VibrationController();
	ShakeDrawObject();
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

	m_vlp_particleTimer = ObjectFactory::Create<RelativeTimer>();
}

void ButiEngine::Player::OnRemove()
{
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

	GUI::BulletText("Acceleration");
	GUI::DragFloat("##accel", &m_acceleration, 0.001f, 0.0f, 1.0f);

	GUI::BulletText("Deceleration");
	GUI::DragFloat("##decel", &m_deceleration, 0.001f, 0.0f, 1.0f);


	GUI::Text("Vibration:%f", m_vibration);

	GUI::BulletText("VibrationForce");
	GUI::DragFloat("##vForce", &m_vibrationForce, 1.0f, 0.0f, 100.0f);

	GUI::BulletText("VibrationIncrease");
	GUI::DragFloat("##vIncrease", &m_vibrationIncrease, 0.001f, 0.0f, 1.0f);

	GUI::BulletText("VibrationDecrease");
	GUI::DragFloat("##vDecrease", &m_vibrationDecrease, 0.001f, 0.0f, 1.0f);
}

void ButiEngine::Player::Start()
{
	CreateDrawObject();

	gameObject.lock()->GetGameComponent<SphereExclusion>()->SetMass(1.0f);

	m_vwp_waveManager = GetManager().lock()->GetGameObject("WaveManager").lock()->GetGameComponent<WaveManager>();

	m_defaultScale = gameObject.lock()->transform->GetLocalScale();
	m_life = 3;

	m_level = 1;
	m_maxLevel = 10;
	m_maxWorkerCount = 5;
	m_exp = 0;

	m_knockBackVelocity = Vector3(0, 0, 0);
	m_knockBackFrame = 0;
	m_maxKnockBackFrame = 15;
	m_isKnockBack = false;

	m_vlp_invincibleTimer = ObjectFactory::Create<RelativeTimer>(60);
	m_isInvincible = false;

	m_isDead = false;

	SetLookAtParameter();
	m_vlp_camera = GetManager().lock()->GetScene().lock()->GetCamera("main");
	m_prevPos = gameObject.lock()->transform->GetLocalPosition();
	m_velocity = Vector3Const::Zero;
	m_maxMoveSpeed = 0.15f;
	m_acceleration = 0.01f;
	m_deceleration = 0.01f;

	m_vwp_shockWave = GetManager().lock()->AddObjectFromCereal("ShockWave");
	m_vwp_shockWave.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);

	auto hzUIParent = GetManager().lock()->AddObjectFromCereal("HzUIParent");
	hzUIParent.lock()->transform->SetLocalPosition(Vector3(-800, -400, 50));
	auto numberManager = GetManager().lock()->AddObjectFromCereal("NumberManager");
	numberManager.lock()->transform->SetBaseTransform(hzUIParent.lock()->transform);
	numberManager.lock()->transform->SetLocalPosition(Vector3(0, 0, 0));
	m_vwp_numberManagerComponent = numberManager.lock()->GetGameComponent<NumberManagerComponent>();
	m_vwp_numberManagerComponent.lock()->SetDigit(2);
	m_vwp_numberManagerComponent.lock()->SetColor(Vector4(1.0f, 0.714f, 0.0f, 1.0f));
	auto hzUI = GetManager().lock()->AddObjectFromCereal("HzUI"); //HzのUI生成
	hzUI.lock()->transform->SetBaseTransform(hzUIParent.lock()->transform);
	hzUI.lock()->transform->SetLocalPosition(Vector3(250, -30, 0));

	hzUIParent.lock()->transform->SetLocalRotationX_Degrees(20);
	hzUIParent.lock()->transform->SetLocalRotationY_Degrees(-15);


	m_isIncrease = false;
	m_isVibrate = false;
	m_vibrationForce = 5.0f;
	m_vibration = 0.0f;
	m_maxVibration = 1.0f;
	m_nearEnemyCount = 0;
	m_vibrationIncrease = 0.024f;
	m_vibrationDecrease = m_maxVibration / 300.0f;
	m_nearEnemyVibrationRate = 0.0f;

	m_vlp_particleTimer->Start();
	m_vlp_particleTimer->ChangeCountFrame(4);
	m_vwp_particleGenerater = GetManager().lock()->GetGameObject("ParticleController").lock()->GetGameComponent<ParticleGenerater>();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Player::Clone()
{
	return ObjectFactory::Create<Player>();
}

void ButiEngine::Player::Dead()
{
	//if (m_vwp_shockWave.lock())
	//{
	//	m_vwp_shockWave.lock()->SetIsRemove(true);
	//}

	if (m_vwp_shockWave.lock())
	{
		m_vwp_shockWave.lock()->GetGameComponent<ShockWave>()->Disappear();
	}

	m_vibration = 0.0f;
	m_isVibrate = false;
	m_vwp_tiltFloatObject.lock()->GetGameComponent<SeparateDrawObject>()->Dead();

	//gameObject.lock()->SetIsRemove(ture);
}

void ButiEngine::Player::Revival()
{
	m_life = 3;
	m_isDead = false;
}

void ButiEngine::Player::AddExp()
{
	if (m_level == m_maxLevel) { return; }

	m_exp++;

	std::uint16_t requestExp = CalculateRequestExp();
	
	//auto beeSoulPodUIComponent = GetManager().lock()->GetGameObject("BeeSoulPod").lock()->GetGameComponent<BeeSoulPodUIComponent>();
	//float expRate = 1.0f - ((float)requestExp - (float)m_exp) * 0.1f;
	//beeSoulPodUIComponent->SetExpRate(expRate);

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

void ButiEngine::Player::SetShockWaveScale(const Vector3& arg_scale)
{
	Vector3 scale = arg_scale / gameObject.lock()->transform->GetLocalScale();
	m_vwp_shockWave.lock()->transform->SetLocalScale(scale);
}

void ButiEngine::Player::Move()
{
	//ノックバック中は操作不能
	if (m_isKnockBack) { return; }
	//リザルト(クリア演出)中は操作不能
	if (m_vwp_waveManager.lock()->IsClearAnimationFlag()) { return; }
	//死んでる間は操作不能
	if (m_isDead) { return; }

	//X、Z平面の移動方向を取得
	Vector2 leftStick = InputManager::GetLeftStick();
	
	auto cameraTransform = m_vlp_camera->vlp_transform;

	if (leftStick != 0)
	{
		Vector3 dir = leftStick.x * cameraTransform->GetRight() + leftStick.y * cameraTransform->GetFront();
		dir.y = 0.0f;

		m_velocity += dir.GetNormalize() * m_acceleration * GameDevice::WorldSpeed;
		if (m_velocity.GetLength() > m_maxMoveSpeed)
		{
			m_velocity = m_velocity.GetNormalize() * m_maxMoveSpeed;
		}

		if (m_vlp_particleTimer->Update())
		{
			auto position = gameObject.lock()->transform->GetWorldPosition();
			Vector3 pachiPachiPosition = position - m_velocity * 10.0f;
			m_vwp_particleGenerater.lock()->TrajectoryParticles(position);
			m_vwp_particleGenerater.lock()->PachiPachiParticles(pachiPachiPosition);
		}

		m_vlp_lookAt->GetLookTarget()->SetLocalPosition((gameObject.lock()->transform->GetLocalPosition() + dir.GetNormalize() * 100.0f));
	}
	else
	{
		if (m_velocity.GetLength() < m_deceleration)
		{
			m_velocity = Vector3Const::Zero;
		}
		m_velocity -= m_velocity.GetNormalize() * m_deceleration * GameDevice::WorldSpeed;
		m_velocity.y = 0;
	}

	if (m_vlp_lookAt->GetLookTarget())
	{
		auto pos = gameObject.lock()->transform->GetLocalPosition();
		auto lookPos = m_vlp_lookAt->GetLookTarget()->GetLocalPosition();
	}

	m_prevPos = gameObject.lock()->transform->GetLocalPosition();
	gameObject.lock()->transform->Translate(m_velocity * GameDevice::WorldSpeed);
}

void ButiEngine::Player::LevelUp()
{
	auto levelUpUI = GetManager().lock()->AddObjectFromCereal("LevelUpUI");
	Vector3 screenPosition = GetCamera("main")->WorldToScreen(gameObject.lock()->transform->GetWorldPosition());
	screenPosition.y += 100;
	screenPosition.z = 0;
	levelUpUI.lock()->transform->SetLocalPosition(screenPosition);

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

void ButiEngine::Player::Damage()
{
	if (m_isDead) { return; }

	m_life--;
	GetManager().lock()->GetGameObject("Camera").lock()->GetGameComponent<CameraShakeComponent>()->ShakeStart(2, 30);

	if (m_life == 0)
	{
		m_isDead = true;
		Dead();
		return;
	}

	m_isInvincible = true;
	m_vlp_invincibleTimer->Start();
}

void ButiEngine::Player::VibrationController()
{
	InputManager::VibrationStop();

	if (m_isDead) { return; }
	if (!m_isVibrate) { return; }

	//float vibrationPower = m_vibration / m_maxVibration;
	InputManager::VibrationStart(m_vibration * 0.3f);
	//InputManager::VibrationStart(m_nearEnemyVibrationRate);
}

void ButiEngine::Player::IncreaseVibration()
{
	if (m_isDead) { return; }

	m_vibration += m_vibrationIncrease * m_nearEnemyCount * GameDevice::WorldSpeed;
	m_vibration = min(m_vibration, m_maxVibration);

	if (!m_isVibrate)
	{
		m_isVibrate = true;
	}
}

void ButiEngine::Player::DecreaseVibration()
{
	if (m_isDead) { return; }
	if (!m_isVibrate) { return; }

	m_vibration -= m_vibrationDecrease * GameDevice::WorldSpeed;

	if (m_vibration <= 0.0f)
	{
		m_vibration = 0.0f;
		m_vwp_shockWave.lock()->GetGameComponent<ShockWave>()->Disappear();
		m_isVibrate = false;
	}
}

void ButiEngine::Player::VibrationEffect()
{
	if (m_isDead) { return; }

	if (m_isVibrate)
	{
		if (m_vwp_vibrationEffect.lock() == nullptr)
		{
			auto transform = gameObject.lock()->transform;
			m_vwp_vibrationEffect = GetManager().lock()->AddObjectFromCereal("VibrationEffect_Player");
			m_vwp_vibrationEffect.lock()->transform->SetLocalPosition(transform->GetLocalPosition());
			m_vwp_vibrationEffect.lock()->transform->SetLocalScale(m_defaultScale * 1.5f);

			m_vwp_vibrationEffectComponent = m_vwp_vibrationEffect.lock()->GetGameComponent<VibrationEffectComponent>();
		}
		else
		{
			auto transform = gameObject.lock()->transform;
			float vibrationRate = m_vibration / m_maxVibration;
			m_vwp_vibrationEffectComponent.lock()->SetVibrationViolent(vibrationRate, true);
			m_vwp_vibrationEffectComponent.lock()->SetEffectPosition(transform->GetLocalPosition());
			m_vwp_shockWave.lock()->GetGameComponent<ShockWave>()->SetScale(vibrationRate);
		}
	}
	else
	{
		StopVibrationEffect();
	}
}

void ButiEngine::Player::StopVibrationEffect()
{
	if (m_isDead) { return; }

	if (m_vwp_vibrationEffect.lock() != nullptr)
	{
		m_vwp_vibrationEffect.lock()->SetIsRemove(true);
		m_vwp_vibrationEffect = Value_weak_ptr<GameObject>();
	}
}

void ButiEngine::Player::VibrationPowerDrawUpdate()
{
	//インスタンスが無い場合通らないようにする
	if (!m_vwp_numberManagerComponent.lock()) { return; }

	auto vibPower = m_vibration / m_maxVibration; //0～1
	std::int32_t vibParcent = vibPower * 100;

	//0～99に補正
	if (vibParcent < 0)
	{
		vibParcent = 0;
	}
	else if (vibParcent >= 100)
	{
		vibParcent = 99;
	}

	m_vwp_numberManagerComponent.lock()->SetNumber(vibParcent);
}

void ButiEngine::Player::ShakeDrawObject()
{
	if (m_isDead) { return; }

	if (!m_vwp_shakeComponent.lock())
	{
		m_vwp_shakeComponent = m_vwp_tiltFloatObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock()->GetGameComponent<ShakeComponent>();
		m_vwp_shakeComponent.lock()->ShakeStart();
		return;
	}

	float vibrationRate = m_vibration / m_maxVibration;
	vibrationRate *= 3.0f;
	if (vibrationRate >= 1.0f)
	{
		vibrationRate = 1.0f;
	}
	m_vwp_shakeComponent.lock()->SetShakePower(vibrationRate);
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

	Vector3 knockBackVelocity = gameObject.lock()->transform->GetLocalPosition() - arg_vwp_other.lock()->transform->GetWorldPosition();
	KnockBack(knockBackVelocity);
	Damage();
}

std::uint16_t ButiEngine::Player::CalculateRequestExp()
{
	return m_level * 10;
}

void ButiEngine::Player::CreateDrawObject()
{
	m_vwp_tiltFloatObject = GetManager().lock()->AddObjectFromCereal("TiltFloatObject");
	m_vwp_tiltFloatObject.lock()->SetObjectName(m_vwp_tiltFloatObject.lock()->GetGameObjectName() + "_" + gameObject.lock()->GetGameObjectName());
	m_vwp_tiltFloatObject.lock()->GetGameComponent<TiltMotion>()->SetParent(gameObject);
	m_vwp_tiltFloatObject.lock()->GetGameComponent<SeparateDrawObject>()->CreateDrawObject("Player");
}

void ButiEngine::Player::SetLookAtParameter()
{
	m_vlp_lookAt = gameObject.lock()->GetGameComponent<LookAtComponent>();
	m_vlp_lookAt->SetLookTarget(gameObject.lock()->transform->Clone());
	m_vlp_lookAt->GetLookTarget()->Translate(gameObject.lock()->transform->GetFront());
	m_vlp_lookAt->SetSpeed(0.05f);
}
