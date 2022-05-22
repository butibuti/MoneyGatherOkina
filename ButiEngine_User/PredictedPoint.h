#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PredictedPoint :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "PredictedPoint";
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

		void SetLife(const std::int32_t arg_life) { m_vlp_lifeTimer->ChangeCountFrame(arg_life); }
	private:
		Value_ptr<Timer> m_vlp_lifeTimer;

	};

}

BUTI_REGIST_GAMECOMPONENT(PredictedPoint, true);

