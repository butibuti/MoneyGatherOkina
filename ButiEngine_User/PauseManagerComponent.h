#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class WorldSpeedManager;
	class ExpansionAnimationComponent;
	class FadeOutComponent;

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
		bool IsNext() { return m_isNext; }
		bool IsBack() { return m_isBack; }

	private:
		void Reset();
		void InputSelect();
		void PauseUI();
		void AppearPauseUI();
		void AppearSelectUI();
		void DeadPauseWindowUI();
		void DeadPauseSelectUI();
		void ScaleAnimation();

		Value_ptr<Timer> m_vlp_waitTimer;
		Value_ptr<Timer> m_vlp_appearTimer;
		Value_ptr<Timer> m_vlp_deadTimer;
		Value_weak_ptr<WorldSpeedManager> m_vwp_worldSpeedManagerComponent;
		Value_weak_ptr<ExpansionAnimationComponent> m_vwp_expansionPauseUIComponent;
		Value_weak_ptr<ExpansionAnimationComponent> m_vwp_expansionPauseWindowUIComponent;
		Value_weak_ptr<FadeOutComponent> m_vwp_fadeOutComponent;
		Value_weak_ptr<GameObject> m_vwp_pauseUI;
		Value_weak_ptr<GameObject> m_vwp_pauseWindowUI;
		Value_weak_ptr<GameObject> m_vwp_pauseBackUI;
		Value_weak_ptr<GameObject> m_vwp_pauseNextSelectUI;
		Value_weak_ptr<GameObject> m_vwp_cursorUI;

		Vector3 m_defaultSelectScale;
		Vector3 m_currentSelectScale;
		Vector3 m_pauseBackScale;
		Vector3 m_pauseNextSelectScale;

		std::int8_t m_nextCount;

		bool m_isPause;
		bool m_isNext;
		bool m_isBack;

	};

}

BUTI_REGIST_GAMECOMPONENT(PauseManagerComponent, true);

