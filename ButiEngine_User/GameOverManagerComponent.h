#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class WorldSpeedManager;

	class GameOverManagerComponent :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "GameOverManagerComponent";
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

		bool IsRetry() { return m_isRetry; }
		bool IsNext() { return m_isNext; }
		bool IsInput() { return m_isInput; }

	private:
		void InputSelect();
		void AppearGameOverUI();
		void AppearSelectUI();

		void ScaleAnimation();
		void PlayerPikuPiku();
		void SelectAnimation();

		Value_ptr<Timer> m_vlp_waitTimer;
		Value_ptr<Timer> m_vlp_pikupikuTimer;
		Value_ptr<Timer> m_vlp_selectAnimationTimer;
		Value_weak_ptr<GameObject> m_vwp_gameOverPlayerUI;
		Value_weak_ptr<GameObject> m_vwp_retryUI;
		Value_weak_ptr<GameObject> m_vwp_nextTitleUI;
		Value_weak_ptr<GameObject> m_vwp_cursorUI;
		Value_weak_ptr<GameObject> m_vwp_selectFlashEffectUI[2];
		Value_weak_ptr<WorldSpeedManager> m_vwp_worldSpeedManagerComponent;

		Vector3 m_defaultSelectScale;
		Vector3 m_retryScale;
		Vector3 m_nextTitleScale;

		std::int8_t m_nextCount;

		bool m_isRetry;
		bool m_isNext;
		bool m_isInput;
		bool m_isSelectAnimation;
	};

}

BUTI_REGIST_GAMECOMPONENT(GameOverManagerComponent, true);

