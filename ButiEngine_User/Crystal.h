#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SeparateDrawObject;

	enum class CrystalPhase
	{
		Appear,
		Wait,
		Disappear
	};

	class Crystal :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Crystal";
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
		void Appear();
		void AppearStart();
		void Wait();
		void WaitStart();
		void Disappeaer();
		void DisappeaerStart();

		void SetEnemyParameter();
		void SetPhaseParameter();
		
		Value_ptr<Timer> m_vlp_lifeTimer;
		Value_ptr<Timer> m_vlp_removeTagTimer;

		CrystalPhase m_phase;

		Value_ptr<Timer> m_vlp_appearTimer;
		Value_ptr<Timer> m_vlp_waitTimer;
		Value_ptr<Timer> m_vlp_disappearTimer;

		Vector3 m_defaultScale;
	};

}

BUTI_REGIST_GAMECOMPONENT(Crystal, true);