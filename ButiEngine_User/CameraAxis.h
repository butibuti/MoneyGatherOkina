#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class CameraAxis : public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "CameraAxis";
		}
		void OnUpdate()override;
		void OnSet()override;
		void OnShowUI()override;
		void Start()override;
		void ChasePlayer(const float arg_lerpScale);
		void RotateAxis(const float arg_lerpScale);
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

	private:
		Value_weak_ptr<GameObject> m_vwp_player;
		Vector3 m_previousRotation;
		Vector3 m_currentRotation;
		Vector3 m_previousPosition;
		Vector3 m_currentPosition;
	};
}

BUTI_REGIST_GAMECOMPONENT(CameraAxis, true);