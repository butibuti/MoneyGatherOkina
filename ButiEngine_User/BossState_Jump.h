#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	enum class JumpPhase
	{
		Wait,
		Rise,
		AirWait,
		Fall
	};

	class BossState_Jump :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "BossState_Jump";
		}
		void OnUpdate()override;
		void OnSet()override;
		void OnRemove()override;
		void OnShowUI()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

	private:
		void Wait();
		void WaitStart();
		void Rise();
		void RiseStart();
		void AirWait();
		void AirWaitStart();
		void Fall();
		void FallStart();

		void SetPhaseParameter();

		Value_ptr<Timer> m_vlp_waitTimer;
		Value_ptr<Timer> m_vlp_riseTimer;
		Value_ptr<Timer> m_vlp_airWaitTimer;
		Value_ptr<Timer> m_vlp_fallTimer;

		JumpPhase m_phase;

		bool m_isStrengthened;
	};

}

BUTI_REGIST_GAMECOMPONENT(BossState_Jump, true);