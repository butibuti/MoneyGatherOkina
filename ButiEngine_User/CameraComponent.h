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
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		void SetZoomOperationNum(const std::int8_t arg_zoomOperationNum) { m_zoomOperationNum = arg_zoomOperationNum; }

	private:
		void CanZoom(const Vector3& arg_zoomPosition, const float arg_rotationX);
		void ZoomOperation(const std::int8_t arg_zoomOperationNum);
		void InputOperationNum();
		void FixOperationNum();
		void CameraReset();

		Value_weak_ptr<GameObject> m_vwp_cameraAxis;

		Vector3 m_defaultPosition;
		Vector3 m_currentPosition;
		Vector3 m_previousPosition;

		float m_defaultRotateX;
		float m_currentRotateX;
		float m_previousRotateX;

		std::int8_t m_zoomOperationNum;
	};
}

BUTI_REGIST_GAMECOMPONENT(CameraComponent, true);