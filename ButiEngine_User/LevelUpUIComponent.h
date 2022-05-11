#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class LevelUpUIComponent :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "LevelUpUIComponent";
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
		void ScaleAnimation();

		Vector3 m_currentScale;
		Vector3 m_previousScale;

		std::int16_t m_life;

		bool m_isChange;

	};

}

BUTI_REGIST_GAMECOMPONENT(LevelUpUIComponent, true);

