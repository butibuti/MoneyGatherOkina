#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class BossState_Crystal :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "BossState_Crystal";
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

		void Dead();
	private:
		Value_ptr<Timer> m_vlp_waitTimer;

		bool m_isStrengthened;
	};

}

BUTI_REGIST_GAMECOMPONENT(BossState_Crystal, true);