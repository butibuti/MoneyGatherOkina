#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class MoveRestriction;

	class Loiter :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Loiter";
		}
		void OnUpdate()override;
		void OnSet()override;
		void OnShowUI()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		void SetMoveRange(const float arg_moveRange) { m_moveRange = arg_moveRange; }
		void SetMaxMoveSpeed(const float arg_moveSpeed) { m_maxMoveSpeed = arg_moveSpeed; }
		void SetAccelFrame(const std::uint8_t arg_accelFrame) { m_vlp_accelTimer->ChangeCountFrame(arg_accelFrame); }
		void SetBrakeFrame(const std::uint8_t arg_brakeFrame) { m_vlp_brakeTimer->ChangeCountFrame(arg_brakeFrame); }
		void SetWaitFrame(const std::uint16_t arg_waitFrame) { m_vlp_waitTimer->ChangeCountFrame(arg_waitFrame); }

		void StartBrake();
		void MoveStart();
		void MoveStop();
	private:
		void Move();
		void Accel();
		void Brake();
		void Wait();
		void SetMoveTarget();

		Value_ptr<MoveRestriction> m_vlp_moveRestriction;
		Value_ptr<Transform> m_vlp_targetSpawner;
		Value_ptr<LookAtComponent> m_vlp_lookAt;
		
		float m_moveRange = 0.0f;
		float m_maxMoveSpeed = 0.0f;

		Vector3 m_velocity;
		Vector3 m_moveTarget;
		float m_moveSpeed;
		float m_speedBeforeBrake;
		bool m_isStop;
		bool m_isMove;
		bool m_isAccel;
		bool m_isBrake;

		Value_ptr<Timer> m_vlp_accelTimer;
		Value_ptr<Timer> m_vlp_brakeTimer;
		Value_ptr<Timer> m_vlp_waitTimer;

		//imguiÇ≈ïœçXÇ∑ÇÈóp
		std::int32_t m_waitFrame = 0;
		std::int32_t m_accelFrame = 0;
		std::int32_t m_brakeFrame = 0;
		
	};

}

BUTI_REGIST_GAMECOMPONENT(Loiter, true);