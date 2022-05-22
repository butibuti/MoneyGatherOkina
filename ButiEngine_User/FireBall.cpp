#include "stdafx_u.h"
#include "FireBall.h"
#include "SpriteParticleGenerator.h"

void ButiEngine::FireBall::OnUpdate()
{
	switch (m_phase)
	{
	case ButiEngine::FireBallPhase::Wait:
		Wait();
		break;
	case ButiEngine::FireBallPhase::Appear:
		Appear();
		break;
	case ButiEngine::FireBallPhase::Rotate:
		Rotate();
		break;
	case ButiEngine::FireBallPhase::Disappear:
		Disappeaer();
		break;
	default:
		break;
	}
}

void ButiEngine::FireBall::OnSet()
{
	auto collisionEnterLambda = std::function<void(Value_weak_ptr<GameObject>&)>([this](Value_weak_ptr<GameObject>& arg_vwp_other)->void
		{
			if (arg_vwp_other.lock()->GetIsRemove()) { return; }

			if (arg_vwp_other.lock()->HasGameObjectTag(GameObjectTag("Player")))
			{
				Dead();
			}
		});

	gameObject.lock()->AddCollisionEnterReaction(collisionEnterLambda);

	SetPhaseParameter();
	SetRotationParameter();

	m_isStrengthened = false;
}

void ButiEngine::FireBall::OnRemove()
{
}

void ButiEngine::FireBall::OnShowUI()
{
}

void ButiEngine::FireBall::Start()
{
	m_vwp_spriteParticleGenerator = GetManager().lock()->GetGameObject("SpriteAnimationParticleController").lock()->GetGameComponent<SpriteParticleGenerator>();

	if (m_isStrengthened)
	{
		m_isClockwise = true;
	}

	gameObject.lock()->transform->SetLocalScale(Vector3Const::Zero);
	m_beforeDisappearScale = m_defaultScale;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FireBall::Clone()
{
	return ObjectFactory::Create<FireBall>();
}

void ButiEngine::FireBall::Dead()
{
	gameObject.lock()->SetIsRemove(true);
}

void ButiEngine::FireBall::Wait()
{
	m_vwp_spriteParticleGenerator.lock()->ChargeParticles(gameObject.lock()->transform->GetLocalPosition(), ButiColor::Purple());

	if (m_vlp_waitTimer->Update())
	{
		AppearStart();
	}
}

void ButiEngine::FireBall::WaitStart()
{
	m_phase = FireBallPhase::Wait;
	m_vlp_waitTimer->Start();
}

void ButiEngine::FireBall::Appear()
{
	float progress = m_vlp_appearTimer->GetPercent() * GameDevice::WorldSpeed;
	progress = min(progress, 1.0f);

	Vector3 scale = MathHelper::LerpPosition(Vector3Const::Zero, m_defaultScale, Easing::EaseOutElastic(progress));
	gameObject.lock()->transform->SetLocalScale(scale);

	if (m_vlp_appearTimer->Update())
	{
		gameObject.lock()->transform->SetLocalScale(m_defaultScale);
		RotateStart();
	}
}

void ButiEngine::FireBall::AppearStart()
{
	m_phase = FireBallPhase::Appear;
	m_vlp_appearTimer->Start();
}

void ButiEngine::FireBall::Rotate()
{
	float rotationDirection = 1.0f;
	if (!m_isClockwise)
	{
		rotationDirection = -1.0f; 
	}

	m_rotationAngle += m_rotationAcceleration * rotationDirection * GameDevice::WorldSpeed;
	if (abs(m_rotationAngle) > m_maxRotationAngle)
	{
		if (m_rotationAngle > 0)
		{
			m_rotationAngle = m_maxRotationAngle;
		}
		else if (m_rotationAngle < 0)
		{
			m_rotationAngle = -m_maxRotationAngle;
		}
	}

	m_vlp_rotationCenter->RollLocalRotationY_Degrees(m_rotationAngle * GameDevice::WorldSpeed);
	if (m_isStrengthened)
	{
		Vector3 scale = m_vlp_rotationCenter->GetLocalScale() * 1.002f * GameDevice::WorldSpeed;
		m_vlp_rotationCenter->SetLocalScale(scale);
		KeepWorldScale();
	}

	if (m_vlp_rotationTimer->Update())
	{
		DisappeaerStart();
	}
}

void ButiEngine::FireBall::RotateStart()
{
	m_phase = FireBallPhase::Rotate;
	m_vlp_rotationTimer->Start();
}

void ButiEngine::FireBall::Disappeaer()
{
	float progress = m_vlp_disappearTimer->GetPercent() * GameDevice::WorldSpeed;
	progress = min(progress, 1.0f);

	Vector3 scale = MathHelper::LerpPosition(m_beforeDisappearScale, Vector3Const::Zero, Easing::EaseInCirc(progress));
	gameObject.lock()->transform->SetLocalScale(scale);

	if (m_vlp_disappearTimer->Update())
	{
		gameObject.lock()->transform->SetLocalScale(Vector3Const::Zero);
		Dead();
	}
}

void ButiEngine::FireBall::KeepWorldScale()
{
	auto parent = gameObject.lock()->transform->GetBaseTransform();
	Vector3 scale = m_defaultScale;
	if (parent)
	{
		scale = scale / parent->GetLocalScale();
	}
	gameObject.lock()->transform->SetLocalScale(scale);
}

void ButiEngine::FireBall::DisappeaerStart()
{
	m_phase = FireBallPhase::Disappear;
	m_vlp_disappearTimer->Start();

	m_beforeDisappearScale = gameObject.lock()->transform->GetLocalScale();
}

void ButiEngine::FireBall::SetPhaseParameter()
{
	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>(120);
	m_vlp_appearTimer = ObjectFactory::Create<RelativeTimer>(30);
	m_vlp_rotationTimer = ObjectFactory::Create<RelativeTimer>(500);
	m_vlp_disappearTimer = ObjectFactory::Create<RelativeTimer>(30);

	WaitStart();
}

void ButiEngine::FireBall::SetRotationParameter()
{
	Vector3 bossPos = GetManager().lock()->GetGameObject(GameObjectTag("Boss")).lock()->transform->GetLocalPosition();
	m_vlp_rotationCenter = ObjectFactory::Create<Transform>(bossPos);

	gameObject.lock()->transform->SetBaseTransform(m_vlp_rotationCenter);

	m_rotationAngle = 0.0f;
	m_maxRotationAngle = 1.0f;
	m_rotationAcceleration = 0.005f;

	if (m_isStrengthened)
	{
		m_maxRotationAngle = 2.0f;
	}
}