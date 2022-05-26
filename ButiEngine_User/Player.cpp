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
#include "VignetteUIComponent.h"
#include "Bomb_Player.h"
#include "GameSettings.h"
#include "FlockingLeader.h"

void ButiEngine::Player::OnUpdate()
{
	if (GameDevice::GetInput()->TriggerKey(Keys::O))
	{
		Damage();
	}

	//if (InputManager::IsTriggerBombKey())
	//{
	//	BombStart();
	//}

	//フロッキング招集モード時、近くに自分より振動値の大きいモブハチがいたら振動値を増やす
	if (m_vwp_flockingLeader.lock()->IsGather() && m_isIncrease)
	{
		//コントローラーの振動値設定
		constexpr float maxControllerVibration = 1.0f;
		m_controllerVibration = MathHelper::Lerp(m_controllerVibration, maxControllerVibration, 0.025f);

		IncreaseVibration();
	}
	else
	{
		//m_controllerVibration = 0.0f;
		DecreaseVibration();
	}
	VibrationEffect();

	//m_nearEnemyCount = 0;
	m_nearWorkerCount = 0;

	MoveKnockBack();
	Move();

	if (m_isInvincible)
	{
		OnInvincible();
	}

	if (m_isOverHeat)
	{
		OverHeat();
	}

	//if (m_isBomb)
	//{
	//	Bomb();
	//}
	VibrationPowerDrawUpdate();
	//VibrationController();
	ShakeDrawObject();
}

void ButiEngine::Player::OnSet()
{
	auto collisionLambda = std::function<void(Value_weak_ptr<GameObject>&)>([this](Value_weak_ptr<GameObject>& arg_vwp_other)->void
		{
			if (arg_vwp_other.lock()->GetIsRemove()) { return; }

			if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("PlayerDamageArea")))
			{
				OnCollisionDamageArea(arg_vwp_other);
			}
			else if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("DamageArea")))
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
	GUI::Text("MaxWorker:%d", m_maxWorkerCount);
	GUI::Text("Life:%d", m_life);

	GUI::BulletText("Speed");
	GUI::DragFloat("##speed", &m_defaultMaxMoveSpeed, 0.01f, 0.0f, 1.0f);

	//GUI::BulletText("BombSpeed");
	//GUI::DragFloat("##bspeed", &m_bombMaxMoveSpeed, 0.01f, 0.0f, 1.0f);

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

	GUI::BulletText("OverHeatFrame");
	if (GUI::DragInt("##overheat", &m_overHeatFrame, 1, 1, 6000))
	{
		m_overHeatTimer->ChangeCountFrame(m_overHeatFrame);
	}
}

void ButiEngine::Player::Start()
{
	CreateDrawObject();

	gameObject.lock()->GetGameComponent<SphereExclusion>()->SetWeight(1.0f);

	m_vwp_waveManager = GetManager().lock()->GetGameObject("WaveManager").lock()->GetGameComponent<WaveManager>();

	m_defaultScale = gameObject.lock()->transform->GetLocalScale();
	m_life = 3;

	m_maxWorkerCount = 20;

	m_vwp_beeSoulPod = GetManager().lock()->GetGameObject("BeeSoulPod").lock()->GetGameComponent<BeeSoulPodUIComponent>();

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
	m_defaultMaxMoveSpeed = 0.25f;
	//m_bombMaxMoveSpeed = 0.4f;
	m_maxMoveSpeed = m_defaultMaxMoveSpeed;
	m_acceleration = 0.1f;
	m_deceleration = 0.1f;

	m_vwp_flockingLeader = GetManager().lock()->GetGameObject("FlockingLeader").lock()->GetGameComponent<FlockingLeader>();

	CreateSensorObject();

	m_vwp_shockWave = GetManager().lock()->AddObjectFromCereal("ShockWave");
	m_vwp_shockWave.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);

	//CreateBombObject();

	m_vwp_hzUIParent = GetManager().lock()->AddObjectFromCereal("HzUIParent");
	m_vwp_hzUIParent.lock()->transform->SetLocalPosition(Vector3(-700, -400, 50));
	m_vwp_numberManager = GetManager().lock()->AddObjectFromCereal("NumberManager");
	m_vwp_numberManager.lock()->transform->SetBaseTransform(m_vwp_hzUIParent.lock()->transform);
	m_vwp_numberManager.lock()->transform->SetLocalPosition(Vector3(0, 0, 0));
	m_defaultNumberUIScale = m_vwp_numberManager.lock()->transform->GetLocalScale();
	m_vwp_numberManagerComponent = m_vwp_numberManager.lock()->GetGameComponent<NumberManagerComponent>();
	m_vwp_numberManagerComponent.lock()->SetDigit(2);
	m_vwp_numberManagerComponent.lock()->SetColor(Vector4(1.0f, 0.714f, 0.0f, 1.0f));
	auto hzUI = GetManager().lock()->AddObjectFromCereal("HzUI"); //HzのUI生成
	hzUI.lock()->transform->SetBaseTransform(m_vwp_hzUIParent.lock()->transform);
	hzUI.lock()->transform->SetLocalPosition(Vector3(250, -30, 0));
	hzUI.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color = GameSettings::SOUL_COLOR;
	m_vwp_maxUI = GetManager().lock()->AddObjectFromCereal("MaxUI"); //MaxのUI生成
	m_vwp_maxUI.lock()->transform->SetBaseTransform(m_vwp_hzUIParent.lock()->transform);
	m_vwp_maxUI.lock()->transform->SetLocalPosition(Vector3(10, 10, 0));
	m_defaultMaxUIScale = m_vwp_maxUI.lock()->transform->GetLocalScale();
	m_vwp_maxUI.lock()->transform->SetLocalScale(Vector3(0, 0, 0));

	m_vwp_hzUIParent.lock()->transform->SetLocalRotationX_Degrees(20);
	m_vwp_hzUIParent.lock()->transform->SetLocalRotationY_Degrees(-15);

	SetVibrationParameter();

	m_vlp_particleTimer->Start();
	m_vlp_particleTimer->ChangeCountFrame(4);
	m_vwp_particleGenerater = GetManager().lock()->GetGameObject("ParticleController").lock()->GetGameComponent<ParticleGenerater>();
	m_vwp_polygonParticleGenerater = GetManager().lock()->GetGameObject("PolygonParticleController").lock()->GetGameComponent<ParticleGenerater>();

	m_vwp_vignetteUI = GetManager().lock()->AddObjectFromCereal("VignetteUI");
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Player::Clone()
{
	return ObjectFactory::Create<Player>();
}

void ButiEngine::Player::Dead()
{
	if (m_vwp_shockWave.lock())
	{
		m_vwp_shockWave.lock()->SetIsRemove(true);
	}

	GetManager().lock()->GetGameObject("Camera").lock()->GetGameComponent<CameraShakeComponent>()->ShakeStart(2, 30);

	auto position = gameObject.lock()->transform->GetWorldPosition();
	m_vwp_polygonParticleGenerater.lock()->ExplosionParticles(position);

	if (m_vwp_sensor.lock())
	{
		m_vwp_sensor.lock()->SetIsRemove(true);
	}

	//if (m_vwp_bomb.lock())
	//{
	//	m_vwp_bomb.lock()->SetIsRemove(true);
	//}

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

void ButiEngine::Player::KnockBack(const Vector3& arg_velocity)
{
	if (m_isKnockBack) return;
	if (m_vwp_waveManager.lock()->IsClearAnimation()) return;

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
	m_prevPos = gameObject.lock()->transform->GetLocalPosition();
	//ノックバック中は操作不能
	if (m_isKnockBack) { return; }
	//リザルト(クリア演出)中は操作不能
	if (m_vwp_waveManager.lock()->IsClearAnimation()) { return; }
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

	gameObject.lock()->transform->Translate(m_velocity * GameDevice::WorldSpeed);
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
	m_vwp_vignetteUI.lock()->GetGameComponent<VignetteUIComponent>()->StartAlphaAnimation();

	if (m_life == 0)
	{
		m_isDead = true;
		m_vwp_vignetteUI.lock()->GetGameComponent<VignetteUIComponent>()->SetIsKeepAlpha(true);
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
	InputManager::VibrationStart(m_controllerVibration);
	//InputManager::VibrationStart(m_nearEnemyVibrationRate);
}

void ButiEngine::Player::IncreaseVibration()
{
	if (m_isDead) { return; }
	if (m_isOverHeat) { return; }

	m_vibration += m_vibrationIncrease * m_nearWorkerCount /*m_nearEnemyCount*/ * GameDevice::WorldSpeed;
	m_vibration = min(m_vibration, m_maxVibration);

	if (GetVibrationRate() >= 1.0f)
	{
		StartOverHeat();

		//meshDraw = m_vwp_bomb.lock()->GetGameComponent<MeshDrawComponent>();
		//meshDraw->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color = GameSettings::ATTACK_COLOR;
	}

	if (!m_isVibrate)
	{
		m_isVibrate = true;
	}
}

void ButiEngine::Player::DecreaseVibration()
{
	if (m_isDead) { return; }
	if (!m_isVibrate) { return; }
	if (m_isOverHeat) { return; }

	m_vibration -= m_vibrationDecrease * GameDevice::WorldSpeed;

	//if (GetVibrationRate() < 1.0f)
	//{
	//	//meshDraw = m_vwp_bomb.lock()->GetGameComponent<MeshDrawComponent>();
	//	//meshDraw->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color = Vector4(0.015f, 0.125f, 0.125f, 0.6f);
	//}

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
			m_vwp_vibrationEffect = GetManager().lock()->AddObjectFromCereal("VibrationEffect");
			m_vwp_vibrationEffect.lock()->transform->SetLocalPosition(transform->GetLocalPosition());
			m_vwp_vibrationEffect.lock()->transform->SetLocalScale(m_defaultScale * 1.5f);

			auto meshDraw = m_vwp_vibrationEffect.lock()->GetGameComponent<MeshDrawComponent>();
			meshDraw->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color = GameSettings::PLAYER_COLOR;

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

	auto vibrationRate = m_vibration / m_maxVibration; //0～1
	std::int32_t vibParcent = vibrationRate * 100;

	if (m_previousVibrationPower < vibrationRate)
	{
		m_vibUpCount++;
	}

	m_previousVibrationPower = vibrationRate;

	//0～99に補正
	if (vibParcent < 0)
	{
		vibParcent = 0;
	}
	else if (vibParcent >= 100)
	{
		//Max
		vibParcent = 99;
		m_vwp_hzUIParent.lock()->transform->SetLocalPosition(Vector3(-700, -400, 50));
		m_vwp_numberManager.lock()->transform->SetLocalScale(Vector3(0, 0, 0));
		m_vwp_numberManagerComponent.lock()->SetScaleAnimationActive(false);
		m_vwp_maxUI.lock()->transform->SetLocalScale(m_defaultMaxUIScale);
		m_isFixNumberUIScale = true;
	}
	else
	{
		if (m_vibUpCount > 15)
		{
			m_vibUpCount = 0;
			//ここでアニメーション開始する関数呼ぶ
			auto upScale = Vector3(m_defaultNumberUIScale.x, m_defaultNumberUIScale.y * 1.5f, m_defaultNumberUIScale.z);
			m_vwp_numberManagerComponent.lock()->ScaleAnimationStart(upScale);
		}
		else if (m_isFixNumberUIScale)
		{
			m_isFixNumberUIScale = false;
			m_vwp_numberManager.lock()->transform->SetLocalScale(m_defaultNumberUIScale);
		}
		m_vwp_hzUIParent.lock()->transform->SetLocalPosition(Vector3(-825, -390, 50));
		m_vwp_maxUI.lock()->transform->SetLocalScale(Vector3(0, 0, 0));
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

void ButiEngine::Player::StartBomb()
{
	if (!m_vwp_bomb.lock()) { return; }
	if (m_isBomb) { return; }
	if (m_vwp_beeSoulPod.lock()->GetSoulRate() < 1.0f) { return; }

	m_maxMoveSpeed = m_bombMaxMoveSpeed;

	auto bomb = m_vwp_bomb.lock()->GetGameComponent<Bomb_Player>();
	bomb->Appear();
	m_vwp_sensor.lock()->transform->SetLocalScale(m_maxSensorScale);

	m_vwp_beeSoulPod.lock()->ResetSoulCount();
	m_vlp_bombTimer->Start();

	m_isBomb = true;
}

void ButiEngine::Player::Bomb()
{
	if (!m_vwp_bomb.lock()) { return; }
	if (m_vlp_bombTimer->Update())
	{
		m_vlp_bombTimer->Stop();

		m_maxMoveSpeed = m_defaultMaxMoveSpeed;

		auto bomb = m_vwp_bomb.lock()->GetGameComponent<Bomb_Player>();
		bomb->Disappear();
		m_vwp_sensor.lock()->transform->SetLocalScale(m_minSensorScale);

		m_isBomb = false;
	}
}

void ButiEngine::Player::StartOverHeat()
{
	m_isOverHeat = true;
	m_overHeatTimer->Start();

	auto meshDraw = m_vwp_tiltFloatObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock()->GetGameComponent<MeshDrawComponent>();
	meshDraw->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color = GameSettings::ATTACK_COLOR;

	if (m_vwp_vibrationEffect.lock())
	{
		meshDraw = m_vwp_vibrationEffect.lock()->GetGameComponent<MeshDrawComponent>();
		meshDraw->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color = GameSettings::ATTACK_COLOR;
	}
}

void ButiEngine::Player::OverHeat()
{
	m_vibration = MathHelper::Lerp(m_vibration, m_overHeatMaxVibration, 0.1f);

	if (m_overHeatTimer->Update())
	{
		m_overHeatTimer->Stop();
		m_overHeatTimer->Reset();

		m_isOverHeat = false;
		m_vibration = 0.0f;

		auto meshDraw = m_vwp_tiltFloatObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock()->GetGameComponent<MeshDrawComponent>();
		meshDraw->GetCBuffer<ButiRendering::ObjectInformation>("ObjectInformation")->Get().color = GameSettings::PLAYER_COLOR;
	}
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
	if (m_isBomb) { return; }
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

void ButiEngine::Player::CreateDrawObject()
{
	m_vwp_tiltFloatObject = GetManager().lock()->AddObjectFromCereal("TiltFloatObject");
	m_vwp_tiltFloatObject.lock()->SetObjectName(m_vwp_tiltFloatObject.lock()->GetGameObjectName() + "_" + gameObject.lock()->GetGameObjectName());
	m_vwp_tiltFloatObject.lock()->GetGameComponent<TiltMotion>()->SetParent(gameObject);
	m_vwp_tiltFloatObject.lock()->GetGameComponent<SeparateDrawObject>()->CreateDrawObject("Player");
}

void ButiEngine::Player::CreateSensorObject()
{
	m_vwp_sensor = GetManager().lock()->AddObjectFromCereal("Sensor");
	m_vwp_sensor.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);
	m_minSensorScale = m_vwp_sensor.lock()->transform->GetLocalScale();
	m_maxSensorScale = Vector3(10.0f, 10.0f, 10.0f);
}

void ButiEngine::Player::CreateBombObject()
{
	m_vlp_bombTimer = ObjectFactory::Create<RelativeTimer>(600);

	m_vwp_bomb = GetManager().lock()->AddObjectFromCereal("Bomb_Player");
	m_vwp_bomb.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);
	m_isBomb = false;
}

void ButiEngine::Player::SetLookAtParameter()
{
	m_vlp_lookAt = gameObject.lock()->GetGameComponent<LookAtComponent>();
	m_vlp_lookAt->SetLookTarget(gameObject.lock()->transform->Clone());
	m_vlp_lookAt->GetLookTarget()->Translate(gameObject.lock()->transform->GetFront());
	m_vlp_lookAt->SetSpeed(0.1f);
}

void ButiEngine::Player::SetVibrationParameter()
{
	m_overHeatFrame = 600;
	m_overHeatTimer = ObjectFactory::Create<RelativeTimer>(m_overHeatFrame);

	m_isIncrease = false;
	m_isVibrate = false;
	m_vibrationForce = 5.0f;
	m_vibration = 0.0f;
	m_maxVibration = 100.0f;
	m_overHeatMaxVibration = m_maxVibration * 4;
	m_nearEnemyCount = 0;
	m_nearWorkerCount = 0;
	m_vibrationIncrease = 0.5f;
	m_vibrationDecrease = 0.02f;
	m_nearEnemyVibrationRate = 0.0f;
	m_isOverHeat = false;
	m_controllerVibration = 0.0f;
	m_previousVibrationPower = 0;
	m_isFixNumberUIScale = false;
}
