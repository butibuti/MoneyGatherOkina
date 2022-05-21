#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class WorldSpeedManager;

	class StageClearManagerComponent :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "StageClearManagerComponent";
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

		bool IsNext() { return m_isNext; }

	private:
		void AddUI();

		Value_ptr<Timer> m_vlp_waitTimer;
		Value_ptr<Timer> m_vlp_addTimer;
		Value_weak_ptr<WorldSpeedManager> m_vwp_worldSpeedManagerComponent;

		std::int8_t m_uiCount;

		bool m_isNext;
	};

}

BUTI_REGIST_GAMECOMPONENT(StageClearManagerComponent, true);

