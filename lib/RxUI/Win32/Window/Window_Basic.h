#pragma once
#include "../UI/UI_Basic.h"


namespace RxUI::Win32::Window
{
	template <class WND_TYPE>
	class Window_Basic : public UI::UI_Basic
	{
	protected:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			Window_Basic* wnd_ptr = nullptr;

			if (uMsg == WM_NCCREATE)
			{
				wnd_ptr = (Window_Basic*)(((CREATESTRUCT*)lParam)->lpCreateParams);
				wnd_ptr->SetHandle(hWnd);
				wnd_ptr->SetUserData((intptr_t)wnd_ptr);
			}
			else
			{
				wnd_ptr = (Window_Basic*)::GetWindowLongPtrW(hWnd, GWLP_USERDATA);
			}

			if (wnd_ptr)
			{
				return wnd_ptr->HandleMsg(uMsg, wParam, lParam);
			}
			else
			{
				return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
		}

		virtual LRESULT HandleMsg(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	public:
		virtual void Create(const wchar_t* wpClass, const wchar_t* wpTtile, uint32_t uiStyle, uint32_t uiStyleEx, HWND hParent) = 0;


	};
}