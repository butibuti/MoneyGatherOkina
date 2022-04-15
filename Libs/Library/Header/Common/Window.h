#pragma once
#include"stdafx.h"

namespace ButiEngine {


	class Window:public IWindow
	{
	public:
		Window();
		~Window();
		void Initialize(const std::string arg_windowName,const WindowPopType arg_popType ,const bool isFullScreen, const std::uint32_t width=0,const std::uint32_t height = 0)override;
		HWND GetHandle()override;
		UInt2 GetSize()override;
		Vector2 GetWindowCenterPosition()override;
		void SetSize(const std::int32_t width,const std::int32_t height)override;
		bool Update()override;
		Vector2 GetWindowPosition()override;
		void Release()override;
		bool IsSelected()override;
		bool TriggerSelected()override;
#ifdef _EDITORBUILD
		bool SetCurrentSelect(const bool arg_isSelect)override;
#endif
	private:
		HWND handle;
		UInt2 size;
		std::wstring wName;
		WNDCLASSW windowClass;
		bool isBefSelect=false, isCurrentSelect=false;
		static LRESULT CALLBACK ProceedMessage(HWND window,	std::uint32_t message, WPARAM wParam, LPARAM lParam);
	};
}