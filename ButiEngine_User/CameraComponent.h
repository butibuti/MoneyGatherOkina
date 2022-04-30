#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class CameraComponent : public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "CameraComponent";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnShowUI()override;
		void ZoomIn();
		void ZoomOut();
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

	private:
		Value_weak_ptr<GameObject> m_vwp_cameraAxis;

		Vector3 m_relativePosition;
		Vector3 m_currentCameraPos;
		Vector3 m_previousCameraPos;

		float m_zoomInFrame;
		float m_zoomOutFrame;
		float m_moveLength;
		float m_initCameraZ;

	};
}

BUTI_REGIST_GAMECOMPONENT(CameraComponent, true);