#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ProgressPointUI :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "ProgressPointUI";
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

		static void ResetZShift() { m_ZShift = m_startZShift; }
	private:
		static float m_startZShift;
		static float m_ZShift;

		Value_ptr<Timer> m_vlp_animatoinTimer;
		Value_ptr<Timer> m_vlp_deadTimer;
		Vector3 m_startScale;
		Vector3 m_targetScale;
		Vector3 m_worldPos;
		float m_positionZ;

		bool m_isDead;
	};

}

BUTI_REGIST_GAMECOMPONENT(ProgressPointUI, true);