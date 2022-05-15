#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class TiltMotion :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "TiltMotion";
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
		float CalculateRotationDirection();

		Value_weak_ptr<GameObject> m_vwp_parent;
		Value_ptr<Transform> m_vlp_lookTarget;
		Vector3 m_front;
		Vector3 m_rotationTarget;
		float m_motionSpeed;
	};

}

BUTI_REGIST_GAMECOMPONENT(TiltMotion, true);
