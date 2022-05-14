#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class StartPopUpComponent :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "StartPopUpComponent";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnShowUI()override;
		void AppearPopUp();
		void DisappearPopUp();
		bool IsHitPlayerFlag();
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		void CheckHitPlayer();
		void ScaleUpdate();

		void SetFloatMotionParameter();

		Value_weak_ptr<GameObject> m_vwp_player;

		Vector3 m_defaultScale;
		Vector3 m_currentScale;
		Vector3 m_previousScale;

		std::int8_t m_knockBackCount;

		bool m_isHitPlayerFlag;
		bool m_scaleAnimationFlag;
	};

}

BUTI_REGIST_GAMECOMPONENT(StartPopUpComponent, true);

