#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManagerComponent;

	class CameraShakeComponent :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "CameraShakeComponent";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		void ShakeStart(const float arg_amplitude);
		void ShakeStart(const float arg_amplitude, const std::int32_t arg_frame);
		void ShakeStop();

	private:
		std::int8_t GetTwoValue();

		Value_weak_ptr<PauseManagerComponent> m_vwp_pauseManagerComponent;

		Vector3 m_moveRotate;
		Vector3 m_subRotate;
		Vector3 m_defaultRotate;
		Vector3 m_startRotate;
		Vector3 m_endRotate;
		float m_amplitude;
		float m_subAmplitude;
		std::int32_t m_oneShakeFrame;
		std::int32_t m_oneShakeCount;
		bool m_startFlag;
	};

}

BUTI_REGIST_GAMECOMPONENT(CameraShakeComponent, true);