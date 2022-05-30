#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class WorldSpeedManager;
	class SoundPlayerComponent;
	class StageProgressUIComponent;
	class NumberManagerComponent;
	class WaveManager;

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
		void AppearProgressBarUI();
		void AppearSelectUI();

		void ScaleAnimation();
		void PlayerPikuPiku();
		void SelectAnimation();

		Value_ptr<Timer> m_vlp_waitTimer;
		Value_ptr<Timer> m_vlp_pikupikuTimer;
		Value_ptr<Timer> m_vlp_addRateTimer;
		Value_ptr<Timer> m_vlp_selectAnimationTimer;
		Value_weak_ptr<GameObject> m_vwp_gameOverPlayerUI;
		Value_weak_ptr<GameObject> m_vwp_retryUI;
		Value_weak_ptr<GameObject> m_vwp_nextTitleUI;
		Value_weak_ptr<GameObject> m_vwp_cursorUI;
		Value_weak_ptr<WorldSpeedManager> m_vwp_worldSpeedManagerComponent;
		Value_weak_ptr<SoundPlayerComponent> m_vwp_soundPlayerComponent;
		Value_weak_ptr<StageProgressUIComponent> m_vwp_progressUIComponent;
		Value_weak_ptr<NumberManagerComponent> m_vwp_restartProgressNumber;
		Value_weak_ptr<WaveManager> m_vwp_waveManagerComponent;

		Vector3 m_defaultSelectScale;
		Vector3 m_retryScale;
		Vector3 m_nextTitleScale;

		std::int8_t m_nextCount;
		std::int8_t m_progressRate;
		std::int8_t m_currentProgressRate;

		bool m_isRetry;
		bool m_isNext;
		bool m_isInput;
		bool m_isSelectAnimation;
		bool m_isAddProgress;
	};

}

BUTI_REGIST_GAMECOMPONENT(GameOverManagerComponent, true);

