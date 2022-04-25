#pragma once
#include"../Device/Keys.h"
#include"../Device/MouseButtons.h"
#include"../Device/PadButtons.h"
namespace ButiEngine {
	class IApplication;
	class Input
	{
		class InputInstance;
	public:
		Input();
		~Input();
		void Initialize(Value_weak_ptr<IApplication> arg_vwp_app);
		bool CheckKey(const std::uint32_t index);
		bool TriggerKey(const std::uint32_t index);
		bool ReleaseKey(const std::uint32_t index);
		bool CheckKey(const Keys index);
		bool TriggerKey(const Keys index);
		bool ReleaseKey(const Keys index);
		bool CheckAnyKey();
		bool TriggerAnyKey();
		void PadUpdate();
		void MouseUpdate();
		bool GetAnyButton();
		bool GetAnyButtonTrigger();
		bool GetPadButton(const PadButtons button);
		bool GetPadButtonTrigger(const PadButtons button);
		bool GetPadButtonRelease(const PadButtons button);
		bool GetMouseButton(const MouseButtons button);
		bool GetMouseTrigger(const MouseButtons button);
		bool GetMouseReleaseTrigger(const MouseButtons button);
		bool GetMouseWheel();
		float GetMouseWheelMove();
		void SetCursorHide(const bool arg_isCursorHide);

		Vector2 GetLeftStick();
		Vector2 GetRightStick();
		float GetLeftTrigger();
		float GetRightTrigger();
		Vector2 GetMouseMove();
		Vector2 GetMousePos();
		void SetMouseCursor(const Vector2& position);
	private:
		HRESULT CreateInput(void);

		HRESULT CreateKey(void);

		HRESULT CreateMouse(void);

		HRESULT SetKeyFormat(void);

		HRESULT SetMouseFormat(void);

		HRESULT SetKeyCooperative(Value_weak_ptr<IApplication> arg_vwp_app);

		HRESULT SetMouseCooperative(Value_weak_ptr<IApplication> arg_vwp_app);



		HRESULT result;


		std::unique_ptr<InputInstance> unq_instance;

		Value_weak_ptr<IApplication> vwp_app;

	};
}
