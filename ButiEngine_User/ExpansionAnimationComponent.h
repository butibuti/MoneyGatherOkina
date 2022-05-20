#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class ExpansionAnimationComponent :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "ExpansionAnimationComponent";
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
		Value_ptr<Timer> m_timer;

		Vector3 m_defaultScale;
		Vector3 m_currentScale;
		Vector3 m_previousScale;
	};

}

BUTI_REGIST_GAMECOMPONENT(ExpansionAnimationComponent, true);

