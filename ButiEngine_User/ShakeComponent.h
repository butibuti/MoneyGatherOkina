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
		Vector3 m_shakeAxis;
		float m_amplitude;
		bool m_isShake;
		bool m_isMove;
		bool m_isShakeLimited;
	};

}

BUTI_REGIST_GAMECOMPONENT(ShakeComponent, true);

