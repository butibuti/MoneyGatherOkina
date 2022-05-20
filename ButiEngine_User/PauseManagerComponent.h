#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class WorldSpeedManager;

	class PauseManagerComponent :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "PauseManagerComponent";
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

		bool IsPause() { return m_isPause; }

	private:
		void Pause();

		Value_weak_ptr<WorldSpeedManager> m_vwp_worldSpeedManagerComponent;

		bool m_isPause;

	};

}

BUTI_REGIST_GAMECOMPONENT(PauseManagerComponent, true);

