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
		void NextScene();
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

	private:
		std::int16_t m_stageNum;
	};
}

BUTI_REGIST_GAMECOMPONENT(StageSelectManagerComponent, true);

