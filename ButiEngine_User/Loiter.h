#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class RigidBodyComponent;

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
		void SetWaitFrame(const std::uint16_t arg_waitFrame) { m_vlp_timer->ChangeCountFrame(arg_waitFrame); }
	private:
		void Move();
		void Wait();
		void SetMoveTarget();

		Value_ptr<RigidBodyComponent> m_vlp_rigidBody;
		Value_ptr<Transform> m_targetSpawner;
		Vector3 m_moveTarget = Vector3Const::Zero;
		float m_moveRange = 0.0f;
		float m_maxMoveSpeed = 0.0f;
		float m_accelFrame = 10;
		float m_brakeFrame = 10;
		bool m_canMove;
		std::int32_t m_testWaitFrame = 0;

		Value_ptr<Timer> m_vlp_timer;
		
	};

}

BUTI_REGIST_GAMECOMPONENT(Loiter, true);