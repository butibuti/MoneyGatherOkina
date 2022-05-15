#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

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
		void AppearUI();

	private:
		void InputSelect();

		void ScaleAnimation(); //��

		Value_ptr<Timer> m_vlp_waitTimer;
		Value_weak_ptr<GameObject> m_vwp_retryUI;
		Value_weak_ptr<GameObject> m_vwp_nextTitleUI;

		Vector3 m_defaultScale;

		bool m_isRetry;
		bool m_isNext;
		bool m_isInput;
	};

}

BUTI_REGIST_GAMECOMPONENT(GameOverManagerComponent, true);

