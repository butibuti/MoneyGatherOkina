#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class SceneChangeAnimationComponent;
	class SoundPlayerComponent;

	class EndSceneManager : public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "EndSceneManager";
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
		void Animation();

		Value_ptr<Timer> m_vlp_waitTimer;
		Value_ptr<Timer> m_vlp_nextTimer;
		Value_weak_ptr<SoundPlayerComponent> m_vwp_soundPlayerComponent;
		Value_weak_ptr<GameObject> m_vwp_endRollUI;
		Value_weak_ptr<GameObject> m_vwp_buttonUI;

		Vector3 m_buttonScale;
		Vector3 m_endScale;

		bool m_isNext;
		bool m_isOnce;
	};
}

BUTI_REGIST_GAMECOMPONENT(EndSceneManager, true);

