#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class StageProgressUIComponent : public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "StageProgressUIComponent";
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
		void SetRate(const float arg_rate) { m_rate = arg_rate; }

	private:
		Value_ptr<Timer> m_vlp_timer;

		Vector4 m_defaultColor;

		float m_rate;
		float m_previousRate;

		bool m_isChangeColor;
	};
}

BUTI_REGIST_GAMECOMPONENT(StageProgressUIComponent, true);

