#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class StageSelectManagerComponent : public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "StageSelectManagerComponent";
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

		void NextScene();
		void BackScene();
		std::int16_t GetStageNum();

	private:
		void FixStageNum();

		std::int16_t m_stageNum;
		std::int16_t m_maxStageNum;
	};
}

BUTI_REGIST_GAMECOMPONENT(StageSelectManagerComponent, true);

