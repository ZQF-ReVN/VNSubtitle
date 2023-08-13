#include "Tools.h"
#include <stdexcept>
#include <dwmapi.h>
#include <versionhelpers.h>
#pragma comment(lib, "dwmapi")


namespace RxUI::Win32::Tools
{
	void GetCenterRect(RECT& refRect, int32_t iWidth, int32_t iHeigh)
	{
		RECT rect_screen;
		GetWindowRect(GetDesktopWindow(), &rect_screen);
		refRect.left = (rect_screen.right - iWidth) / 2;
		refRect.top = (rect_screen.bottom - iHeigh) / 2;
		refRect.right = refRect.left + iWidth;
		refRect.bottom = refRect.top + iHeigh;
	}

	// https://github.com/glfw/glfw/blob/master/src/win32_window.c
	bool EnableAlphaCompositing(HWND hWnd)
	{
		if (!::IsWindowsVistaOrGreater()) { return false; }

		BOOL is_composition_enable = false;
		if (FAILED(::DwmIsCompositionEnabled(&is_composition_enable))) { throw std::runtime_error("Dwm Check Composition Error!"); }
		if (!is_composition_enable) { return true; }

		DWORD current_color = 0;
		BOOL is_opaque = false;
		if (FAILED(::DwmGetColorizationColor(&current_color, &is_opaque))) { throw std::runtime_error("Dwm Check Color Error!"); }

		if (!is_opaque || IsWindows8OrGreater())
		{
			HRGN region = ::CreateRectRgn(0, 0, -1, -1);
			DWM_BLURBEHIND bb = { 0 };
			bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
			bb.hRgnBlur = region;
			bb.fEnable = TRUE;
			::DwmEnableBlurBehindWindow(hWnd, &bb);
			::DeleteObject(region);
			return true;
		}
		else // For Window7
		{
			DWM_BLURBEHIND bb = { 0 };
			bb.dwFlags = DWM_BB_ENABLE;
			::DwmEnableBlurBehindWindow(hWnd, &bb);
			return false;
		}
	}

	// WS_EX_LAYERED 会对性能造成影响，但是暂时没找到不开启该属性在DWM下的鼠标穿透方法
	bool EnableMouseClickThrough(HWND hWnd)
	{
		::SetWindowLongPtrW(hWnd, GWL_EXSTYLE, ::GetWindowLongPtrW(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT);
		return ::SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);
	}
}