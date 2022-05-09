#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class InputManager :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "InputManager";
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

		//ゲーム全体
		static bool IsPushRightKey();
		static bool IsPushLeftKey();
		static bool IsPushUpKey();
		static bool IsPushDownKey();
		static bool IsTriggerRightKey();
		static bool IsTriggerLeftKey();
		static bool IsTriggerUpKey();
		static bool IsTriggerDownKey();
		static bool IsTriggerDecideKey();
		static bool IsTriggerCancelKey();
		static bool IsZoomInKey();
		static bool IsZoomOutKey();
		static bool IsCameraResetKey();
		static bool IsPushAnyGameKey();

		static void VibrationStart(const float arg_vibPower);
		static void VibrationStop();

		//ゲームプレイシーンのみ
		static bool IsPushConveneKey();
		static bool IsTriggerPauseKey();

		static Vector2 GetRightStick();
		static Vector2 GetLeftStick();
		static float GetRightTrigger();
		static float GetLeftTrigger();
		static bool IsNotPushKey() { return m_isNotPushKey; }
	private:
		static const float m_DEADZONE;
		static bool m_isNotPushKey;
		static Vector2 m_previousRightStick;
		static Vector2 m_currentRightStick;
		static Vector2 m_previousLeftStick;
		static Vector2 m_currentLeftStick;
	};

}

BUTI_REGIST_GAMECOMPONENT(InputManager, true);