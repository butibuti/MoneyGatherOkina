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

	private:
		std::int32_t m_life;

	};

}

BUTI_REGIST_GAMECOMPONENT(PredictedPoint, true);

