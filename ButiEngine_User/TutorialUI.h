#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class TutorialUI :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "TutorialUI";
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

		void Appear();
	private:
		bool m_isAppear;
		bool m_isLoopAnim;
		Value_ptr<Timer> m_vlp_appearTimer;
		Vector3 m_startScale;
		Vector3 m_targetScale;
		float m_theta;
	};

}

BUTI_REGIST_GAMECOMPONENT(TutorialUI, true);