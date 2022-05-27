#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class ShakeComponent :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "ShakeComponent";
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

		void SetShakeAxis(const Vector3& arg_shakeAxis) { m_shakeAxis = arg_shakeAxis; }
		void Shake(const float arg_amplitude, const std::int32_t arg_shakeFrame);
		void SetRotateAmplitude(const float arg_amplitude) { m_rotateAmplitude = arg_amplitude; }
		void SetPositionAmplitude(const float arg_amplitude) { m_positionAmplitude = arg_amplitude; }
		void SetScaleAmplitude(const float arg_amplitude) { m_scaleAmplitude = arg_amplitude; }
		void SetIsRotateShake(const bool arg_isShake) { m_isRotateShake = arg_isShake; }
		void SetIsPositionShake(const bool arg_isShake) { m_isPositionShake = arg_isShake; }
		void SetIsScaleShake(const bool arg_isShake) { m_isScaleShake = arg_isShake; }
		void SetIsAbsolute(const bool arg_isAbsolute) { m_isAbsolute = arg_isAbsolute; }
		void ShakeStart();
		void ShakeStop();
		void SetShakePower(const float arg_amplitude);

	private:
		void ShakeLimited();

		Value_ptr<Timer> m_vlp_shakeTimer;

		Vector3 m_currentRotate;
		Vector3 m_moveRotate;
		Vector3 m_currentPos;
		Vector3 m_movePos;
		Vector3 m_currentScale;
		Vector3 m_moveScale;
		Vector3 m_shakeAxis;
		float m_amplitude;
		float m_rotateAmplitude;
		float m_positionAmplitude;
		float m_scaleAmplitude;
		bool m_isShake;
		bool m_isMove;
		bool m_isShakeLimited;
		bool m_isRotateShake;
		bool m_isPositionShake;
		bool m_isScaleShake;
		bool m_isAbsolute;
	};

}

BUTI_REGIST_GAMECOMPONENT(ShakeComponent, true);

