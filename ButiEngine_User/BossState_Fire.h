#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class BossState_Fire :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "BossState_Fire";
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

		void EndState();
	private:
		void CreateFireBall();

		Value_ptr<Timer> m_vlp_stateTimer;

		std::vector<Value_weak_ptr<GameObject>> m_vec_fireBalls;

		bool m_isStrengthened;
	};

}

BUTI_REGIST_GAMECOMPONENT(BossState_Fire, true);